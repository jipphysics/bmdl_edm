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
#include "cpp_graph.h"
#include "cpp_edm.h"
#include "cpp_mc_sampling.h"

/*
int main() {
        
    std::cout << "Creating a graph and adding links to it..." << std::endl;

    Graph G;
    G.add_link( 1 , 2 );    
    G.add_link( 1 , 3 );    
    G.add_link( 2 , 3 );    
    G.add_link( 3 , 4 );    
    G.add_link( 4 , 5 );    
    G.add_link( 4 , 6 );    
    G.add_link( 5 , 6 );    

    double beta_edm = .16;
    std::cout << "beta_edm = " << beta_edm << std::endl;

    std::cout << "G = " << G << std::endl;
    
    std::cout << "Creating a edm..." << std::endl;
    
    ExternalDegreeModel edm( G );    
    
    std::cout << "edm = " << edm << std::endl;
    std::cout << "edm.j( beta_edm ) = " << edm.j( beta_edm ) << std::endl;
    std::cout << "edm.j( beta_edm ) = " << edm.J( beta_edm ) << std::endl;

    std::cout << "Running Annealing in edm..." << std::endl;

    edm.set_one_label_all_nodes();  
    MC_Sampler mc_sampler( edm , beta_edm );
    double beta_prime = 1.;
    std::cout << "beta_prime = " << patch::to_string( beta_prime ) << std::endl;
    int max_num_MC_rounds = 100;
    int MC_steps_per_MC_round = 10;
    int MC_step = 0;
    for ( int num_MC_rounds = 0 ; num_MC_rounds < max_num_MC_rounds ; num_MC_rounds++ ) {
        std::cout << "num_MC_rounds = " << patch::to_string( num_MC_rounds ) << " MC_step = " << patch::to_string( MC_step ) << " E = " << mc_sampler.E() << std::endl;
        MC_step += mc_sampler.run_mc_sampler( beta_prime , MC_steps_per_MC_round );
    }

    std::cout << "edm = " << edm << std::endl;
    std::cout << "mc_sampler.edm() = " << mc_sampler.edm() << std::endl;
    std::cout << "edm.j( beta_edm ) = " << edm.j( beta_edm ) << std::endl;
    std::cout << "edm.J( beta_edm ) = " << edm.J( beta_edm ) << std::endl;
    std::cout << "mc_sampler.edm().j( beta_edm ) = " << mc_sampler.edm().j( beta_edm ) << std::endl;
    std::cout << "mc_sampler.edm().J( beta_edm ) = " << mc_sampler.edm().J( beta_edm ) << std::endl;
  
}
*/
