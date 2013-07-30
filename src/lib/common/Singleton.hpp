#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

#include "NonCopyable.hpp"

namespace Common
{
    template<class T>
    class Singleton : public NonCopyable
    {
        public:
			
            static T& instance()
            {
                static T single_instance;
		        return single_instance;
            }
        
        protected:

            Singleton(){}
            ~Singleton(){}
		
    };

} // Commin

#endif // _SINGLETON_HPP_
