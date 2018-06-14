#include <cassert>
#include <cstddef>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
//#include <cstdlib>     // std::rand, std::srand
#include "to_string_patch.h"
#include "cpp_runningstats.h"
 
// === For testing purposes ===.

/*
int main() {

    Graph G;
        
    std::cout << "Creating a graph and adding links to it..." << std::endl;
    
    G.add_link( 1 , 2 );    
    G.add_link( 1 , 3 );    
    G.add_link( 2 , 3 );    
    G.add_link( 3 , 4 );    
    G.add_link( 4 , 5 );    
    G.add_link( 4 , 6 );    
    G.add_link( 5 , 6 );    

    double beta = 1.;
    std::cout << "beta = " << beta << std::endl;

    std::cout << "G = " << G << std::endl;

    for ( Graph::iterator i_it = G.begin() ; i_it != G.end() ; i_it++ ) {
        int i = i_it.idx();
        std::cout << "i = " << patch::to_string( i ) << " G.degree( i ) = " << patch::to_string( G.degree( i ) ) << std::endl; 
    }
    
    std::cout << "Creating a edm..." << std::endl;
    
    ExcessDegreeModel edm = ExcessDegreeModel( G );

    for ( Graph::iterator i_it = G.begin() ; i_it != G.end() ; i_it++ ) {
        int i = i_it.idx();
        std::cout << "i = " << patch::to_string( i ) << " edm.excess_degree( i ) = " << patch::to_string( edm.excess_degree( i ) ) << std::endl; 
    }
        
    std::cout << "edm = " << edm << std::endl;
    std::cout << "edm.num_intra_links() = " << edm.num_intra_links() << std::endl;    
    std::cout << "edm.num_inter_links() = " << edm.num_inter_links() << std::endl;    
    std::cout << "edm.MDL_1st() = " << edm.MDL_1st() << std::endl;
    std::cout << "edm.MDL_2nd() = " << edm.MDL_2nd() << std::endl;
    std::cout << "edm.MDL( beta ) = " << edm.MDL( beta ) << std::endl;

    std::cout << "Modifying some node's labels..." << std::endl;
    
    edm.set_node_label( 1 , 10 );
    edm.set_node_label( 3 , 10 );

    edm.set_node_label( 4 , 20 );
    edm.set_node_label( 6 , 20 );

    std::cout << "edm = " << edm << std::endl;
    std::cout << "edm.num_intra_links() = " << edm.num_intra_links() << std::endl;    
    std::cout << "edm.num_inter_links() = " << edm.num_inter_links() << std::endl;    
    std::cout << "edm.MDL_1st() = " << edm.MDL_1st() << std::endl;
    std::cout << "edm.MDL_2nd() = " << edm.MDL_2nd() << std::endl;
    std::cout << "edm.MDL( beta ) = " << edm.MDL( beta ) << std::endl;

    std::cout << "Setting the best labeling..." << std::endl;

    edm.set_node_label( 1 , 10 );
    edm.set_node_label( 2 , 10 );
    edm.set_node_label( 3 , 10 );

    edm.set_node_label( 4 , 20 );
    edm.set_node_label( 5 , 20 );
    edm.set_node_label( 6 , 20 );

    std::cout << "edm = " << edm << std::endl;
    std::cout << "edm.G().num_links() = " << edm.G().num_links() << std::endl;    
    std::cout << "edm.num_intra_links() = " << edm.num_intra_links() << std::endl;    
    std::cout << "edm.num_inter_links() = " << edm.num_inter_links() << std::endl;    
    std::cout << "edm.MDL_1st() = " << edm.MDL_1st() << std::endl;
    std::cout << "edm.MDL_2nd() = " << edm.MDL_2nd() << std::endl;
    std::cout << "edm.MDL( beta ) = " << edm.MDL( beta ) << std::endl;
    
    for ( Graph::iterator i_it = G.begin() ; i_it != G.end() ; i_it++ ) {
        int i = i_it.idx();
        std::cout << "i = " << patch::to_string( i ) << " edm.excess_degree( i ) = " << patch::to_string( edm.excess_degree( i ) ) << " edm.avrg_excess_degree( i ) = " << patch::to_string( edm.avrg_excess_degree( i ) ) << std::endl; 
    }

    std::set< int > subnodes;
    subnodes.insert( 1 );
    subnodes.insert( 2 );
    subnodes.insert( 3 );        
    ExcessDegreeModel subedm( edm , subnodes );    
    std::cout << "subedm = " << subedm << std::endl;
    std::cout << "subedm.num_intra_links() = " << subedm.num_intra_links() << std::endl;    
    std::cout << "subedm.num_inter_links() = " << subedm.num_inter_links() << std::endl;    
    std::cout << "subedm.MDL_1st() = " << subedm.MDL_1st() << std::endl;
    std::cout << "subedm.MDL_2nd() = " << subedm.MDL_2nd() << std::endl;
    std::cout << "subedm.MDL( beta ) = " << subedm.MDL( beta ) << std::endl;   
    
}
*/
