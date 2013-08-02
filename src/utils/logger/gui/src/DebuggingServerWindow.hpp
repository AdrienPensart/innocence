#pragma once

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
