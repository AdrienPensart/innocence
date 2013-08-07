#pragma once

#include <QDialog>
#include <ui_DebuggingConsole.h>
#include "LogThread.hpp"
#include <common/Message.hpp>
using Common::Message;

class LogGuiWindow : public QDialog, public Ui::DebuggingDialog
{
	Q_OBJECT
       
	public:
       
		LogGuiWindow(QWidget * parent = 0);
           
	public slots:
       
		void addMessage(Message message);
		void saveLog();

	private:

		LogThread logThread;
};
