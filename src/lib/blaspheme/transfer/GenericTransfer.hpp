#pragma once

#include <system/File.hpp>
#include "FileTransfer.hpp"

namespace Blaspheme
{
    class NetworkStream
    {
        Network::Stream& stream;
        size_t offset;

        public:
            
            NetworkStream(Network::Stream & _stream)
                :stream(_stream), offset(0)
            {
            }
            
            // retourne le nombre d'octets effectivement reçus ou envoyes sur le réseau
            size_t size()
            {
                return offset;
            }
            
            // ecrit les octets en memoire a l'offset courant
            // a partir du contenu de input_chunk
            // retourne le nombre d'octets effectivement ecris
            size_t write(const std::vector<char>& input_chunk)
            {
                size_t coffset = stream.send(&input_chunk[0], input_chunk.size());
                offset += coffset;
                return coffset;
            }
            
            // lit les octets en memoire a l'offset courant et
            // place le contenu dans output_chunk
            // retourne le nombre d'octets effectivement lus
            size_t read(std::vector<char>& output_chunk)
            {
                size_t coffset = stream.recv(&output_chunk[0], output_chunk.size());
                offset += coffset;
                return coffset;
            }
    };

    class Memory
    {
        // l'offset en cours de lecture / ecriture
        size_t offset;

        // morceau de memoire input / output
        std::vector<char> chunk;

        public:

            Memory(size_t initSize)
            {
                chunk.resize(initSize);
            }

            size_t size()
            {
                return chunk.size();
            }

            // ecrit les octets en memoire a l'offset courant
            // a partir du contenu de input_chunk
            // retourne le nombre d'octets effectivement ecris
            size_t write(const std::vector<char>& input_chunk, size_t towrite)
            {
                return 0;
            }
            
            // lit les octets en memoire a l'offset courant et
            // place le contenu dans output_chunk
            // retourne le nombre d'octets effectivement lus
            size_t read(std::vector<char>& output_chunk)
            {
                
                return 0;
            }
    };

    class File
    {
        // chemin du fichier
        std::string filepath;
        std::fstream filehandle;
        // taille du fichier a la fin du transfert
        std::streamsize filesize;
        // offset a laquel lire / ecrire le fichier
        size_t offset;

        public:

            File(const std::string& _filepath)
                :filepath(_filepath)
            {
                filehandle.open(filepath.c_str(),  std::ios::in | std::ios::out | std::ios::binary);
                if (filehandle.is_open())
                {
                    LOG << "Can't open file " + filepath;
                    throw FileNotFound();
                }
                filesize = System::Size(filehandle);
            }

            ~File()
            {
            }
            
            std::streamsize size()
            {
                return filesize;
            }
            
            // ecrit les octets en memoire a l'offset courant
            // a partir du contenu de input_chunk
            // retourne le nombre d'octets effectivement ecris
            size_t write(const std::vector<char>& input_chunk)
            {
                return 0;
            }
            
            // lit les octets en memoire a l'offset courant et
            // place le contenu dans output_chunk
            // retourne le nombre d'octets effectivement lus
            size_t read(std::vector<char>& output_chunk)
            {
                return 0;
            }
    };

    // type de transfert : upload, download
    // type de flux : memory, file
    template <class Input, class Output>
    class GenericTransfer
    {
        enum {BUFFER_SIZE = 40000};

        public:
            
            GenericTransfer(Input& _input, Output& _output)
                :input(_input), output(_output)
            {
                buffer.resize(BUFFER_SIZE);
            }

            void launch()
            {
                
            }

        private:
            
            TransferDescriptor td;
            Input& input;
            Output& output;
            std::vector<char> buffer;
    };

    typedef GenericTransfer<NetworkStream, NetworkStream> Proxy;
    typedef GenericTransfer<Memory, Memory> MemCopy;
    typedef GenericTransfer<File, File> FileCopy;
    typedef GenericTransfer<File, NetworkStream> FileUpload;
    typedef GenericTransfer<NetworkStream, File> FileDownload;
    typedef GenericTransfer<Memory, NetworkStream> MemUpload;
    typedef GenericTransfer<NetworkStream, Memory> MemDownload;
    typedef GenericTransfer<File, Memory> FileLoader;
    typedef GenericTransfer<Memory, File> FileFlush;

} // Blaspheme
