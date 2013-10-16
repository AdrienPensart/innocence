#pragma once

#ifdef WIN32

#include <network/Timeout.hpp>
#include <common/WindowsWrapper.hpp>

namespace System
{
	class Thread
	{
		public:

			Thread(LPTHREAD_START_ROUTINE routine, LPVOID parameter=0);
			virtual ~Thread();
			virtual void start();
            virtual void stop();
			virtual bool join(const Network::Timeout& deadline=Network::Timeout::infinite());
			bool isRunning();

		protected:

			void start(LPTHREAD_START_ROUTINE routine);

		private:

			LPTHREAD_START_ROUTINE routine;
			LPVOID parameter;
			DWORD dwThread;
            HANDLE hThread;
			bool running;
	};
} // System

#endif // WIN32