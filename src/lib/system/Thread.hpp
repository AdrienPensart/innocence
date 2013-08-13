#pragma once

#ifdef WIN32

#include <network/Timeout.hpp>
#include <windows.h>

namespace System
{
	class Thread
	{
		public:

			Thread(LPTHREAD_START_ROUTINE routine);
			virtual ~Thread();
			virtual void start();
            virtual void stop();
			virtual bool join(const Network::Timeout& deadline=Network::Timeout::infinite());
			bool isRunning();

		protected:

			void start(LPTHREAD_START_ROUTINE routine);

		private:

			LPTHREAD_START_ROUTINE routine;
			DWORD dwThread;
            HANDLE hThread;
			bool running;
	};
} // System

#endif // WIN32