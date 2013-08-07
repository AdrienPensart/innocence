#include <QFileDialog>
#include <fstream>

#include "LogGuiWindow.hpp"
#include "LogThread.hpp"

LogGuiWindow::LogGuiWindow(QWidget * parent) : 
	QDialog(parent)
{
	setupUi(this);
	connect(saveLogButton, SIGNAL(clicked()), this, SLOT(saveLog()));
	QObject::connect(&logThread, SIGNAL(newMessage(Message)), this, SLOT(addMessage(Message)));
	logThread.start();
}

void LogGuiWindow::addMessage(Common::Message message)
{
	QString msg = QString::fromStdString(message.getContent());
	if(msg[msg.size()-1] == '\n')
	{
		msg.chop(1);
	}
	debugOutputEdit->append(msg);
}

void LogGuiWindow::saveLog()
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
