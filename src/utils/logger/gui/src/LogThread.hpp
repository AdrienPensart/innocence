#pragma once

#include <QThread>
#include <common/LogServer.hpp>
#include <common/Message.hpp>
using Common::Message;

class LogThread : public QThread
{
	Q_OBJECT
       
	public:
       
		LogThread();
		~LogThread();
		void run();
        void addMessage(Message message);

	signals:
        
		void newMessage(Message message);
            
	private:
       
		Common::LogServer logserver;
};

class LogToGui : public Common::LogObserver
{
    public:

        LogToGui(LogThread * logThread);
        virtual void update(const Common::Message& message);

    private:
	
		LogThread * logThread;
};
