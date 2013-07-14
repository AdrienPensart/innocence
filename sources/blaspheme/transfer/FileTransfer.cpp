#include "FileTransfer.hpp"

#include <common/Logger.hpp>
#include <system/File.hpp>
#include <blaspheme/Blaspheme.hpp>
#include <blaspheme/hash/Hash.hpp>

namespace Blaspheme
{
    TransferDescriptor::TransferDescriptor()
    {
        transferred = 0;
        totalsize = 0;
        percentage = 0;
    }
    
    // met a jour la progression
    bool TransferDescriptor::update()
    {
        if(totalsize != 0)
        {
            unsigned int current_percentage = (((unsigned int)transferred * 100) / (unsigned int)totalsize);
            if(current_percentage > percentage)
            {
                percentage = current_percentage;
                return true;
            }
        }
        return false;
    }

    void LogTransfer::update(const TransferDescriptor& td)
    {
		LOG << to_string(td.percentage);
	}

    Transfer::Transfer(const std::string& _filename, Network::Stream& _stream)
		:stream(_stream), state(NOT_STARTED)
    {
        filename = _filename;
    }

    void Transfer::launch()
    {
        init();
        while(!isDone())
        {
            transfer();
        }
        finish();
    }

    bool Transfer::isDone()
    {
        if(td.transferred > td.totalsize)
        {
            throw TransferOverflow();
        }
        return (td.transferred == td.totalsize);
    }

    Download::Download(const std::string& _filename, Network::Stream& _stream)
        :Transfer(_filename, _stream)
    {
    }

    void Download::init()
    {
        try
        {
            file.open(filename.c_str(), std::ios::out | std::ios::binary);
            if (file.is_open())
            {
                std::string buffer_size;
                stream.recv(hash, '\n', false);
                
                if(hash == FAILURE)
                {
                    // l'envoyeur n'a pas trouve le fichier a envoyer
                    state = CANCELED;
                    throw RemoteFileNotFound();
                }
                
                stream.recv(buffer_size, '\n', false);
                from_string(buffer_size, td.totalsize);
                state = INITIALIZED;
                notify(td);
            }
            else
            {
                state = CANCELED;
                LOG << "Can't open file " + filename;
                throw LocalFileNotFound();
            }
        }
        catch(...)
        {
            state = CANCELED;
            throw;
        }
    }

    void Download::transfer()
    {
        try
        {
            static std::vector<char> buffer(40000);
            int recvd = stream.recv(&buffer[0], buffer.size());
            if(recvd <= 0)
            {
                LOG << "Probleme : RawRecv a retourne " + to_string(recvd);
                throw TransferException();
            }
            
            file.write(&buffer[0], recvd);
            td.transferred += recvd;
            if(td.update())
            {
                notify(td);
            }
        }
        catch(...)
        {
            state = CANCELED;
            throw;
        }
    }

    void Download::finish()
    {
        if(file.is_open())
        {
            file.close();
        }
   
        Hash hasher;
        std::string destination_hash = hasher.getHashFromFile(filename);                
        // on verifie si le hash du fichier est correct
        if(destination_hash != hash)
        {
            LOG << "Le hash du fichier est different, le fichier est altere.";
        }
        else if(td.totalsize == td.transferred)
        {
            td.percentage = 100;
            notify(td);
        }
        state = DONE;
    }

    InMemoryDownload::InMemoryDownload(std::string& output_buffer, Network::Stream& input_stream)
        :Transfer("", input_stream), memory_buffer(output_buffer)
    {
    }

    void InMemoryDownload::init()
    {
        // reception du hash
        std::string buffer_size;
        stream.recv(hash, '\n', false);
        if(hash == FAILURE)
        {
            // l'envoyeur n'a pas trouve le fichier a envoyer
            state = CANCELED;
            throw RemoteFileNotFound();
        }

        // reception de la taille
        stream.recv(buffer_size, '\n', false);
        from_string(buffer_size, td.totalsize);
        state = INITIALIZED;
        notify(td);

        // pour optimiser le transfert, on va tout de suite réserver 
        // la capacité mémoire adéquate, attention au chargement de fichiers
        // trop gros...
        memory_buffer.reserve(td.totalsize);
    }

    void InMemoryDownload::transfer()
    {
        try
        {
            static std::vector<char> buffer(40000);
            int recvd = stream.recv(&buffer[0], buffer.size());
            if(recvd <= 0)
            {
                LOG << "Probleme : RawRecv a retourne " + to_string(recvd);
                throw TransferException();
            }
            
            memory_buffer.append(&buffer[0], recvd);
            td.transferred += recvd;
            if(td.update())
            {
                notify(td);
            }
        }
        catch(...)
        {
            state = CANCELED;
            throw;
        }
    }
    
    void InMemoryDownload::finish()
    {
        Hash hasher;
        std::string destination_hash = hasher.getHash(memory_buffer);
        if(destination_hash != hash)
        {
            LOG << "Le hash initial n'est pas le meme que le hash final.";
        }
        else if(td.totalsize == td.transferred)
        {
            td.percentage = 100;
            notify(td);
        }
        state = DONE;
    }

    Upload::Upload(const std::string& _filename, Network::Stream& _stream)
    :Transfer(_filename, _stream)
    {
    }

    void Upload::init()
    {
        try
        {
            LOG << "Ouverture du fichier a envoyer...";
            file.open(filename.c_str(), std::ios::in | std::ios::binary);
            if(file)
            {
				td.totalsize = System::Size(file);
                Blaspheme::Hash hasher;
                hash = hasher.getHashFromFile(filename);
                stream.send(hash+'\n');
                stream.send(to_string(td.totalsize)+'\n');
                LOG << "Fichier ouvert taille = " + to_string(td.totalsize) +", hash = " + hash;
                state = INITIALIZED;
                LOG << "Transfert initialise.";
            }
            else
            {
                LOG << "Echec de l'ouverture de " + filename;
                // On doit notifier au receveur de fichier que le fichier ne peut pas 
                // etre ouvert
                stream << FAILURE;
                throw LocalFileNotFound();
            }
        }
        catch(...)
        {
            state = CANCELED;
            throw;
        }
    }

    void Upload::transfer()
    {
        static std::vector<char> buffer(40000);
        file.read(&buffer[0], buffer.size());
        std::streamsize to_send = file.gcount();
        int sent = 0;
        
        while(sent != to_send)
        {
            int temp_sent = stream.send(&buffer[0]+sent, to_send-sent);
            if(temp_sent <= 0)
            {
				LOG << "Probleme RawSend a retourne : " + to_string(temp_sent) + " <=> " + Network::SocketException::getLastError();
                throw TransferException();
            }
            sent += temp_sent;
        }

        td.transferred += sent;
        if(td.update())
        {
            notify(td);
        }
    }

    void Upload::finish()
    {
        if(file.is_open())
        {
            file.close();
            state = DONE;
        }
        
        if(td.totalsize == td.transferred)
        {
            td.percentage = 100;
        }
        notify(td);
    }

} /* Blaspheme */
