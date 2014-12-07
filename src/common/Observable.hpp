#pragma once

#include <list>

namespace Common
{
	template <typename Observer, typename Information>
	class Observable
	{
		std::list<Observer *> observers;

		public:

			~Observable()
			{
				for(typename std::list<Observer *>::iterator iter = observers.begin(); iter != observers.end(); ++iter)
				{
					delete *iter;
				}
			}

			void notify(const Information& information)
			{
				for (typename std::list<Observer *>::iterator iter = observers.begin(); iter != observers.end(); ++iter)
				{
					(*iter)->update(information);
				}
			}

			void addObserver(Observer * observer)
			{
				observers.push_back(observer);
			}

			bool isObserved()
			{
				return !observers.empty();
			}
	};

} // Common
