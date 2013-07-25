#include <QFileDialog>
#include <fstream>

#include "DebuggingServerWindow.hpp"
#include "DebugThread.hpp"

namespace Debugger
{
	DebuggingServerWindow::DebuggingServerWindow(QWidget * parent)
	:QDialog(parent)
	{
		setupUi(this);
		connect(saveLogButton, SIGNAL(clicked()), this, SLOT(saveLog()));
	}

	void DebuggingServerWindow::addDebugMsg(QString msg)
	{
		if(msg[msg.size()-1] == '\n')
		{
			msg.chop(1);
		}
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

} // Debugger
