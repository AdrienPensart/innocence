#include "Timeout.hpp"

#include <climits>

namespace Network
{
	Timeout::Timeout(const long& sec, const long& usec)
	{
	    time.tv_sec = sec;
		time.tv_usec = usec;
	}

	void Timeout::set(const long& sec, const long& usec)
	{
		time.tv_sec = sec;
		time.tv_usec = usec;
	}

	timeval & Timeout::c_struct()
	{
		return time;
	}

	unsigned long Timeout::getMs() const
	{
		return time.tv_sec * 1000 + time.tv_usec;
	}

	const Timeout& Timeout::infinite()
	{
		static Timeout maxT(LONG_MAX, LONG_MAX);
		return maxT;
	}
}
