#ifndef _INTERNET_EXPLORER_
#define _INTERNET_EXPLORER_

#include <exdisp.h>
#include <windows.h>

namespace Malicious
{
	class InternetExplorer
	{
		public:
		
			InternetExplorer();
			DWORD getPid(){return pid;}
			void kill();

		private:

			IWebBrowser2 * browser;
			DWORD pid;
	};

	// too dangerous, high risk of crash
	//void killEveryIE();
}

#endif // _INTERNET_EXPLORER_
