#ifndef _NON_COPYABLE_HPP_
#define _NON_COPYABLE_HPP_

namespace Common
{
    class NonCopyable
    {
		public:
			NonCopyable() {}
		  
		private:

			NonCopyable( const NonCopyable& that ) {}
			NonCopyable& operator=( const NonCopyable& that ) { return *this; }
    };
} // Common 

#endif // _NON_COPYABLE_HPP_
