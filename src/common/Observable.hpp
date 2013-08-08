#pragma once

#include <list>

namespace Common
{
    template <class Observer, class Information>
    class Observable
    {
        std::list<Observer *> observers;

        public:
            
            ~Observable()
            {
                for(std::list<Observer *>::iterator iter = observers.begin(); iter != observers.end(); ++iter)
                {
                    delete *iter;
                }
            }

            void notify(const Information& information)
            {
                for (std::list<Observer *>::iterator iter = observers.begin(); iter != observers.end(); ++iter)
                {
                    (*iter)->update(information);
                }
            }

            void addObserver(Observer * observer)
            {
                observers.push_back(observer);
            }
    };
	
} // Common
