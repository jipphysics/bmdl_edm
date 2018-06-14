#include <cassert>
#include <cstddef>
#include <vector>
//#include <set>
//#include <map>
#include <iostream>
#include <string>
#include <sstream>
//#include <algorithm>    // std::random_shuffle
//#include <ctime>        // std::time
//#include <cstdlib>     // std::rand, std::srand
//#include "cpp_static_graph.h"
#include "cpp_edm.h"  
#include "to_string_patch.h"

// === For testing purposes ===.


int main() {

    std::cout << "Creating a StaticGraph and adding links to it..." << std::endl;

    std::set< std::pair< int , int > > links;
    std::pair< int , int > p1 = std::make_pair( 0 , 1 );
    links.insert( p1 );
    std::pair< int , int > p2 = std::make_pair( 0 , 2 );
    links.insert( p2 );
    std::pair< int , int > p3 = std::make_pair( 1 , 2 );
    links.insert( p3 );
    std::pair< int , int > p4 = std::make_pair( 2 , 3 );
    links.insert( p4 );
    std::pair< int , int > p5 = std::make_pair( 3 , 4 );
    links.insert( p5 );
    std::pair< int , int > p6 = std::make_pair( 3 , 5 );
    links.insert( p6 );
    std::pair< int , int > p7 = std::make_pair( 4 , 5 );
    links.insert( p7 );
    StaticGraph G( links );

    std::cout << "G = " << G << std::endl;
    for ( StaticGraph::t_node_iterator i_it = G.node_it() ; i_it ; ++i_it ) {
        int i = ( * i_it );
        std::cout << "i = " << i << " G.degree( i ) = " << G.degree( i ) << std::endl; 
    }
   
    std::cout << std::endl; 
    std::cout << "Creating a edm..." << std::endl;
   
    ExternalDegreeModel edm = ExternalDegreeModel( G );

    for ( StaticGraph::t_node_iterator i_it = G.node_it() ; i_it ; ++i_it ) {
        int i = ( * i_it );
        std::cout << "i = " << i << " edm.external_degree( i ) = " << edm.external_degree( i ) << std::endl; 
    }
        
    double beta = 1.;
    std::cout << "beta = " << beta << std::endl;

    std::cout << "edm = " << edm << std::endl;
    std::cout << "edm.num_intra_links() = " << edm.num_intra_links() << std::endl;    
    std::cout << "edm.num_inter_links() = " << edm.num_inter_links() << std::endl;    
    std::cout << "edm.effective_Hamiltonian_ord0() = " << edm.effective_Hamiltonian_ord0() << std::endl;
    std::cout << "edm.effective_Hamiltonian_ord1() = " << edm.effective_Hamiltonian_ord1() << std::endl;
    std::cout << "edm.effective_Hamiltonian( beta ) = " << edm.effective_Hamiltonian( beta ) << std::endl;
    std::cout << "edm.effective_L( beta ) = " << edm.effective_L( beta ) << std::endl;
    std::cout << "edm.mdl( beta ) = " << edm.mdl( beta ) << std::endl;

    std::cout << std::endl;
    std::cout << "Modifying some node's labels..." << std::endl;
    
    edm.set_node_label( 0 , 0 );
    edm.set_node_label( 1 , 0 );
    edm.set_node_label( 2 , 0 );

    edm.set_node_label( 3 , 1 );
    edm.set_node_label( 4 , 1 );
    edm.set_node_label( 5 , 1 );

    std::cout << "edm = " << edm << std::endl;
    std::cout << "edm.num_intra_links() = " << edm.num_intra_links() << std::endl;    
    std::cout << "edm.num_inter_links() = " << edm.num_inter_links() << std::endl;    
    std::cout << "edm.effective_Hamiltonian_ord0() = " << edm.effective_Hamiltonian_ord0() << std::endl;
    std::cout << "edm.effective_Hamiltonian_ord1() = " << edm.effective_Hamiltonian_ord1() << std::endl;
    std::cout << "edm.effective_Hamiltonian( beta ) = " << edm.effective_Hamiltonian( beta ) << std::endl;
    std::cout << "edm.effective_L( beta ) = " << edm.effective_L( beta ) << std::endl;
    std::cout << "edm.mdl( beta ) = " << edm.mdl( beta ) << std::endl;

    std::cout << std::endl;
    std::cout << "Updating quantities..." << std::endl;

    edm.update_quantities();

    std::cout << "edm = " << edm << std::endl;
    std::cout << "edm.num_intra_links() = " << edm.num_intra_links() << std::endl;    
    std::cout << "edm.num_inter_links() = " << edm.num_inter_links() << std::endl;    
    std::cout << "edm.effective_Hamiltonian_ord0() = " << edm.effective_Hamiltonian_ord0() << std::endl;
    std::cout << "edm.effective_Hamiltonian_ord1() = " << edm.effective_Hamiltonian_ord1() << std::endl;
    std::cout << "edm.effective_Hamiltonian( beta ) = " << edm.effective_Hamiltonian( beta ) << std::endl;
    std::cout << "edm.effective_L( beta ) = " << edm.effective_L( beta ) << std::endl;
    std::cout << "edm.mdl( beta ) = " << edm.mdl( beta ) << std::endl;

    std::cout << std::endl;
    std::cout << "Setting a ''bad'' labeling..." << std::endl;

    edm.set_node_label( 0 , 2 );
    edm.set_node_label( 1 , 1 );
    edm.set_node_label( 2 , 1 );
    edm.set_node_label( 3 , 1 );
    edm.set_node_label( 4 , 2 );
    edm.set_node_label( 5 , 3 );

    std::cout << "edm = " << edm << std::endl;
    std::cout << "edm.num_intra_links() = " << edm.num_intra_links() << std::endl;    
    std::cout << "edm.num_inter_links() = " << edm.num_inter_links() << std::endl;    
    std::cout << "edm.effective_Hamiltonian_ord0() = " << edm.effective_Hamiltonian_ord0() << std::endl;
    std::cout << "edm.effective_Hamiltonian_ord1() = " << edm.effective_Hamiltonian_ord1() << std::endl;
    std::cout << "edm.effective_Hamiltonian( beta ) = " << edm.effective_Hamiltonian( beta ) << std::endl;
    std::cout << "edm.effective_L( beta ) = " << edm.effective_L( beta ) << std::endl;
    std::cout << "edm.mdl( beta ) = " << edm.mdl( beta ) << std::endl;
    
    for ( StaticGraph::t_node_iterator i_it = G.node_it() ; i_it ; ++i_it ) {
        int i = ( * i_it );
        std::cout << "i = " << i << " edm.external_degree( i ) = " << edm.external_degree( i ) << " edm.avrg_external_degree( i ) = " << patch::to_string( edm.avrg_external_degree( i ) ) << std::endl; 
    }

}


