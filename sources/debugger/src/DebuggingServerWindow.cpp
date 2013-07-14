#include <QFileDialog>
#include <iostream>
#include <fstream>

#include "DebuggingServerWindow.hpp"

DebugThread::DebugThread(Network::Port default_port, QWidget * parent)
:QThread(parent), terminating(false)
{
    server.listen(default_port);
}  
DebugThread::~DebugThread()
{
    terminating = true;
    wait();
}
void DebugThread::run()
{
    try
    {
        Network::Timeout deadline(0, 100);
        std::string buffer;
        while(!terminating)
        {
            server.recv(buffer, deadline);
            if(buffer.size())
            {
                std::cout << buffer;
                QString q_buffer(buffer.c_str());
                emit newDebugMsg(q_buffer);
            }
        }
    }
    catch(...)
    {
    }
}

DebuggingServerWindow::DebuggingServerWindow(QWidget * parent)
:QDialog(parent)
{
    setupUi(this);
    connect(saveLogButton, SIGNAL(clicked()), this, SLOT(saveLog()));
}
void DebuggingServerWindow::addDebugMsg(QString msg)
{
    msg[msg.size()-1] = ' ';
    debugOutputEdit->append(msg);
}
void DebuggingServerWindow::saveLog()
{
    if(debugOutputEdit->toPlainText().toStdString().size())
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"C:\\",tr("Text (*.txt)"));
        if(fileName.size())
        {   
            std::fstream file;
            file.open(fileName.toStdString().c_str(), std::ios::out | std::ios::app);
            file.write(debugOutputEdit->toPlainText().toStdString().c_str(), debugOutputEdit->toPlainText().toStdString().size());
            file.close();
        }
    }
}
