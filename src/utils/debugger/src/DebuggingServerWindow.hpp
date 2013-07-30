#ifndef _DEBUGGING_SERVER_WINDOW_
#define _DEBUGGING_SERVER_WINDOW_

#include <QDialog>
#include <ui_DebuggingConsole.h>

namespace Debugger
{
	class DebuggingServerWindow : public QDialog, public Ui::DebuggingDialog
	{
		Q_OBJECT
        
		public:
        
			DebuggingServerWindow(QWidget * parent = 0);
            
		public slots:
        
			void addDebugMsg(QString msg);
			void saveLog();
	};

} // Debugger

#endif // _DEBUGGING_SERVER_WINDOW_