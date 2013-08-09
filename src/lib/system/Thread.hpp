#pragma once

#ifdef WIN32

#include <windows.h>

namespace System
{
	class Thread
	{
		public:
			virtual void start()=0;
            virtual void stop();
			bool isRunning();

		protected:
			void start(LPTHREAD_START_ROUTINE routine);

		private:
			DWORD dwThread;
            HANDLE hThread;
			bool running;
	};
} // System

#endif // WIN32