#include <cassert>
#include <cstddef>
#include <set>
#include <map>
#include <iostream>
#include <string>
#include <sstream> 
#include <fstream>    // ifstream
#include "to_string_patch.h"
#include "cpp_static_graph.h"
   
// === For testing purposes ===.


const StaticGraph load_StaticGraph( std::string infilename ) {
    std::set< int > nodes;
    std::set< std::pair< int , int > > links;
    std::ifstream infile( infilename.c_str() );   
    std::string line;
    while ( std::getline( infile , line ) ) {
        std::istringstream iss( line );
        int i , j;
        if ( !( iss >> i >> j ) ) { break; } // error
        if ( i > j ) {
            int tmp = i;
            i = j;
            j = tmp;
        }
        i--;
        j--;
        std::pair< int , int > p = std::make_pair( i , j );
        nodes.insert( i );
        nodes.insert( j );
        links.insert( p );
    }
    return StaticGraph( nodes.size() , links );
}

/*
const StaticGraph load_StaticGraphWithLinkAttributes( std::string infilename ) {
    std::set< int > nodes;
    std::set< Link > links;
    std::ifstream infile( infilename.c_str() );   
    std::string line;
    while ( std::getline( infile , line ) ) {
        std::istringstream iss( line );
        int i , j , a;
        if ( !( iss >> i >> j ) ) { break; } // error
        if ( i > j ) {
            int tmp = i;
            i = j;
            j = tmp;
        }
        i--;
        j--;
        a = std::max( i , j ); // For the test, we fake the index attribute.
        Link lnk( i , j , a );
        nodes.insert( i );
        nodes.insert( j );
        links.insert( lnk );
    }
    return StaticGraph( nodes.size() , nodes.size() , links );
}
*/

int main() {
 
    StaticGraph G = load_StaticGraph( "karate.pairs" );
    //StaticGraph G = load_StaticGraphWithLinkAttributes( "karate.pairs" );
    std::cout << G << std::endl;

    std::cout << "G.has_link( 31 , 28 ) = " << G.has_link( 31 , 28 ) << std::endl;
    std::cout << "G.has_link( 31 , 29 ) = " << G.has_link( 31 , 29 ) << std::endl;

}


