#pragma once

#include <common/WindowsWrapper.hpp>
#include <exdisp.h>

namespace Malicious {
	class InternetExplorer {
		public:

			InternetExplorer(bool killOnDestroy=false);
			~InternetExplorer();

			DWORD getPid()const;
			void kill();

		private:

			IWebBrowser2 * browser;
			DWORD pid;
			bool killOnDestroy;
	};
}
