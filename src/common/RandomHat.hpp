#pragma once

#include <vector>
#include <ctime>
#include <cstdlib>
#include <common/WindowsWrapper.hpp>

namespace Common
{
	inline int gettimeofday(struct timeval * tp, struct timezone * tzp)
	{
		static const unsigned __int64 epoch = 116444736000000000;
		FILETIME    file_time;
		SYSTEMTIME  system_time;
		ULARGE_INTEGER ularge;
 
		GetSystemTime(&system_time);
		SystemTimeToFileTime(&system_time, &file_time);
		ularge.LowPart = file_time.dwLowDateTime;
		ularge.HighPart = file_time.dwHighDateTime;
 
		tp->tv_sec = (long) ((ularge.QuadPart - epoch) / 10000000L);
		tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
		return 0;
	}

	class HatInitializer
	{
		public:
			HatInitializer()
			{
				timeval ts;
				gettimeofday(&ts,0);
				srand((ts.tv_sec * 1000 + (ts.tv_usec / 1000)));
			}
	};

	template<class T>
	class Hat : public std::vector<T>
	{
		public:
			void mix()
			{
				std::vector<T> hatTemp;
				for(size_t index = size(); index > 0; index--)
				{
					hatTemp.push_back(randomPop());
				}
				assign(hatTemp.begin(), hatTemp.end());
			}

			// retire un élément de la pile aléatoirement
			T randomPop()
			{
				if(!size())
					return T();
				T temp;
				unsigned int index = random(0, size()-1);
				temp = at(index);
				erase(begin()+index);
				return temp;
			}

			// tire un élément de la pile, mais en le laissant dans la pile
			T randomPull()
			{
				if(!hat.size())
					return T();
				return hat[random(0, hat.size()-1)];
			}

			unsigned int random(int nLow, int nHigh)
			{
				return (rand() % (nHigh - nLow + 1)) + nLow;
			}
	};
	static HatInitializer hi;
} // Common
