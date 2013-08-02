#pragma once

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
}
