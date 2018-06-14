#ifndef Included_to_string_patch
#define Included_to_string_patch

#include <string>
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
    
}

#endif // Included_to_string_patch
