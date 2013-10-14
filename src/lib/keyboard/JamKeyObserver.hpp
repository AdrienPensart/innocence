#pragma once

#include "KeyObserver.hpp"

#include <vector>

namespace Keyboard
{
	typedef std::vector<int> KeyCombination;
	class JamKeyObserver : public KeyObserver
	{
		public:
			JamKeyObserver(const KeyCombination& keyComb);
			virtual void update(const KeyInformation& ki);
			
		private:
			KeyCombination keyComb;
	};
} // Keyboard
