#include "Timeout.hpp"

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
}
