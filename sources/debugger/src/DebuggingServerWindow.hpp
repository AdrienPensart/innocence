#include <QThread>
#include <QDialog>
#include <network/UdpSocket.hpp>
#include <ui_DebuggingConsole.h>

class DebugThread : public QThread
{
    Q_OBJECT
        
    public:
        
        DebugThread(Network::Port default_port, QWidget * parent = 0);
        ~DebugThread();
        void run();
            
    signals:
        
        void newDebugMsg(QString msg);
            
    private:
        
        Network::UdpSocket server;
        bool terminating;
};
    
class DebuggingServerWindow : public QDialog, public Ui::DebuggingDialog
{
    Q_OBJECT
        
    public:
        
        DebuggingServerWindow(QWidget * parent = 0);
            
    public slots:
        
        void addDebugMsg(QString msg);
        void saveLog();
};
