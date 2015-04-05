#include <QFileDialog>
#include <fstream>

#include "LogGuiWindow.hpp"
#include "LogThread.hpp"

namespace LogGui {
	LogGuiWindow::LogGuiWindow(QWidget * parent) :
		QDialog(parent) {
		setupUi(this);
		connect(saveLogButton, SIGNAL(clicked()), this, SLOT(saveLog()));
		qRegisterMetaType<Message>("Message");
		QObject::connect(&logThread, SIGNAL(newMessage(Message)), this, SLOT(addMessage(Message)));
		logThread.start();
	}

	void LogGuiWindow::addMessage(Log::Message message) {
		std::string bufferMsg = message.getIdentity().getModule() + " -> (" + message.getLine() + " in " + message.getFile() + ") (" + message.getCallStack() + ") : " + message.getContent();
		debugOutputEdit->append(QString::fromStdString(bufferMsg));

		const int currentRow = messagesTableWidget->rowCount();
		messagesTableWidget->insertRow(currentRow);
		messagesTableWidget->setItem(currentRow, 0, new QTableWidgetItem(QString::fromStdString(message.getIdentity().getModule())));
		messagesTableWidget->setItem(currentRow, 1, new QTableWidgetItem(QString::fromStdString(message.getIdentity().getComputer())));
		messagesTableWidget->setItem(currentRow, 2, new QTableWidgetItem(QString::fromStdString(message.getIdentity().getIp())));
		messagesTableWidget->setItem(currentRow, 3, new QTableWidgetItem(QString::fromStdString(message.getIdentity().getBuildId())));
		messagesTableWidget->setItem(currentRow, 4, new QTableWidgetItem(QString::fromStdString(message.getIdentity().getBuildDate())));
		messagesTableWidget->setItem(currentRow, 5, new QTableWidgetItem(QString::fromStdString(message.getIdentity().getExecutionDate())));
		messagesTableWidget->setItem(currentRow, 6, new QTableWidgetItem(QString::fromStdString(message.getIdentity().getOs())));
		messagesTableWidget->setItem(currentRow, 7, new QTableWidgetItem(QString::fromStdString(message.getFile())));
		messagesTableWidget->setItem(currentRow, 8, new QTableWidgetItem(QString::fromStdString(message.getLine())));
		messagesTableWidget->setItem(currentRow, 9, new QTableWidgetItem(QString::fromStdString(message.getTime())));
		messagesTableWidget->setItem(currentRow, 10, new QTableWidgetItem(QString::fromStdString(message.getCallStack())));
		messagesTableWidget->setItem(currentRow, 11, new QTableWidgetItem(QString::fromStdString(message.getContent())));
	}

	void LogGuiWindow::saveLog() {
		if(debugOutputEdit->toPlainText().toStdString().size()) {
			QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"C:\\",tr("Text (*.txt)"));
			if(fileName.size()) {
				std::fstream file;
				file.open(fileName.toStdString().c_str(), std::ios::out | std::ios::app);
				file.write(debugOutputEdit->toPlainText().toStdString().c_str(), debugOutputEdit->toPlainText().toStdString().size());
				file.close();
			}
		}
	}
} // LogGui
