#include <cstddef>
//#include <set>
//#include <map>
//#include <vector>
//#include <array>        // std::array
#include <iostream>
#include <cassert>
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include "cpp_utils.h"

/*
int main() {

    int i;

    std::cout << "Enumerate::Example 1" << std::endl;

    CPP_Enumerate< int > enu;
 
    i = enu.insert( 3 );
    i = enu.insert( 2 );
    i = enu.insert( 10 );
    i = enu.insert( 2 );
    i = enu.insert( 2 );
    i = enu.insert( 5 );
    i = enu.insert( 3 );

    std::cout << "enu.size()="    << enu.size()    << std::endl;
    std::cout << "enu.get( 3  )=" << enu.get( 3  ) << std::endl;
    std::cout << "enu.get( 2  )=" << enu.get( 2  ) << std::endl;
    std::cout << "enu.get( 10 )=" << enu.get( 10 ) << std::endl;
    std::cout << "enu.get( 5  )=" << enu.get( 5  ) << std::endl;

    std::cout << "Enumerate::Example 2" << std::endl;

    CPP_Enumerate< std::pair< int , int > > enu2;
 
    i = enu2.insert( std::make_pair< int , int >( 1 , 2 ) );
    i = enu2.insert( std::make_pair< int , int >( 1 , 3 ) );
    i = enu2.insert( std::make_pair< int , int >( 2 , 1 ) );
    i = enu2.insert( std::make_pair< int , int >( 1 , 2 ) );
    i = enu2.insert( std::make_pair< int , int >( 1 , 3 ) );
    i = enu2.insert( std::make_pair< int , int >( 2 , 3 ) );

    std::cout << "enu.size()="    << enu2.size()    << std::endl;
    std::cout << "enu.get( std::make_pair( 1 , 2 ) )=" << enu2.get( std::make_pair( 1 , 2 ) ) << std::endl;
    std::cout << "enu.get( std::make_pair( 1 , 3 ) )=" << enu2.get( std::make_pair( 1 , 3 ) ) << std::endl;
    std::cout << "enu.get( std::make_pair( 2 , 1 ) )=" << enu2.get( std::make_pair( 2 , 1 ) ) << std::endl;
    std::cout << "enu.get( std::make_pair( 2 , 3 ) )=" << enu2.get( std::make_pair( 2 , 3 ) ) << std::endl;

}
*/
