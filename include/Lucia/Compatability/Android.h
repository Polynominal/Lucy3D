#ifndef MIKUS_LUCY_ANDROID_H_INCLUDED
#define MIKUS_LUCY_ANDROID_H_INCLUDED
#include <string>
#include <sstream>
#ifdef __ANDROID__
namespace std
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream ss;
        ss << n ;
        return ss.str() ;
    }
}
namespace Lucia
{
    namespace Compat
    {
        void patch(){};
    }
}
#endif

#endif // LUCY_ANDROID_H_INCLUDED
