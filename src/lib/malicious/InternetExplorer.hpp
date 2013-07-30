#ifndef _INTERNET_EXPLORER_
#define _INTERNET_EXPLORER_

#include <exdisp.h>
#include <windows.h>

namespace Malicious
{
	class InternetExplorer
	{
		public:
		
			InternetExplorer(bool killOnDestroy=false);
			~InternetExplorer();

			DWORD getPid();
			void kill();

		private:

			IWebBrowser2 * browser;
			DWORD pid;
			bool killOnDestroy;
	};

	// too dangerous, high risk of crash
	//void killEveryIE();
}

#endif // _INTERNET_EXPLORER_
