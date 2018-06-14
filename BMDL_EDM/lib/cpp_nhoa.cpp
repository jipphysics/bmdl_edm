#include <cstddef>
#include <set>
#include <map>
#include <vector>
#include <array>        // std::array
#include <iostream>
#include <cassert>
#include <algorithm>    // std::random_shuffle (uses std::rand) , std::shuffle (uses a random num. generator, e.g. std::mt19937)
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <random>       // std::mt19937
#include <functional>   // std::ref
//#include "cpp_utils.h"
//#include "cpp_graph.h"
#include "cpp_static_graph.h"
//#include "cpp_partition.h"
#include "cpp_edm.h"
#include "cpp_nhoa.h"

// === Class NhoaState ===
          
// === Function toolboox ===    
            
// === For testing purposes ===

int main() {
      
    /*  
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
    */
 
    std::cout << "Loading 'karate-club' as StaticGraph..." << std::endl;

    StaticGraph G = load_StaticGraph( "karate.pairs" ); 
    std::cout << G << std::endl;

    double beta = .16;
    std::cout << "beta = " << beta << std::endl;
    std::cout << "G = " << G << std::endl;
    std::cout << std::endl;    

    std::cout << "Creating a edm..." << std::endl;
    
    ExternalDegreeModel edm( G );    
    
    std::cout << "edm = " << edm << std::endl;
    std::cout << "edm.effective_Hamiltonian( beta ) = " << edm.effective_Hamiltonian( beta ) << std::endl;
    std::cout << "edm.Lambda( beta ) = " << edm.Lambda( beta ) << std::endl;
    std::cout << "edm.mdl( beta ) = " << edm.mdl( beta ) << std::endl;
    std::cout << std::endl;    

    std::cout << "Running NHOA in edm..." << std::endl;
  
    edm.set_one_label_per_node();  
    //run_nhoa( edm , beta , 20000 , 0.000000001 , true , true );
    ExternalDegreeModel best_edm( edm );
    double best_E = sample_nhoa( edm , best_edm , beta , 20000 , 0.000000001 , true , 3 );

    //std::cout << "edm = " << edm << std::endl;
    //std::cout << "edm.effective_Hamiltonian( beta ) = " << edm.effective_Hamiltonian( beta ) << std::endl;
    //std::cout << "edm.Lambda( beta ) = " << edm.Lambda( beta ) << std::endl;
    //std::cout << "edm.mdl( beta ) = " << edm.mdl( beta ) << std::endl;

    std::cout << "best_edm = " << best_edm << std::endl;
    std::cout << "best_edm.effective_Hamiltonian( beta ) = " << best_edm.effective_Hamiltonian( beta ) << std::endl;
    std::cout << "best_edm.Lambda( beta ) = " << best_edm.Lambda( beta ) << std::endl;
    std::cout << "best_edm.mdl( beta ) = " << best_edm.mdl( beta ) << std::endl;
  
}
