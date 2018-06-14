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
#include "cpp_meta_edm.h"  
#include "to_string_patch.h"

// === For testing purposes ===.

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

    //std::cout << std::endl;
    //std::cout << "Modifying some node's labels..." << std::endl;
    
    //edm.set_node_label( 0 , 0 );
    //edm.set_node_label( 1 , 0 );
    //edm.set_node_label( 2 , 0 );

    //edm.set_node_label( 3 , 1 );
    //edm.set_node_label( 4 , 1 );
    //edm.set_node_label( 5 , 1 );
        
    double beta = 1.;
    std::cout << "beta = " << beta << std::endl;

    std::cout << "edm = " << edm << std::endl;
    std::cout << "edm.num_intra_links() = " << edm.num_intra_links() << std::endl;    
    std::cout << "edm.num_inter_links() = " << edm.num_inter_links() << std::endl;    
    std::cout << "edm.effective_Hamiltonian_ord1() = " << edm.effective_Hamiltonian_ord1() << std::endl;
    std::cout << "edm.effective_Hamiltonian_ord2() = " << edm.effective_Hamiltonian_ord2() << std::endl;
    std::cout << "edm.mdl( beta ) = " << edm.mdl( beta ) << std::endl;

    std::cout << "Creating a meta_edm..." << std::endl;

    meta_EDM medm( edm , beta );
    
    std::cout << medm << std::endl;    

    std::cout << "Creating the ground truth partition..." << std::endl;

    Partition ground_truth_partition( edm.G().num_nodes() );
    // FOR toy network.
    //ground_truth_partition.change_label_of_element( 0 , 0 );
    //ground_truth_partition.change_label_of_element( 1 , 0 );
    //ground_truth_partition.change_label_of_element( 2 , 0 );
    //ground_truth_partition.change_label_of_element( 3 , 1 );
    //ground_truth_partition.change_label_of_element( 4 , 1 );
    //ground_truth_partition.change_label_of_element( 5 , 1 );
    // FOR karate-club network.
    ground_truth_partition.change_label_of_element( 0 , 0 );
    ground_truth_partition.change_label_of_element( 1 , 0 );
    ground_truth_partition.change_label_of_element( 2 , 0 );
    ground_truth_partition.change_label_of_element( 3 , 0 );
    ground_truth_partition.change_label_of_element( 4 , 0 );
    ground_truth_partition.change_label_of_element( 5 , 0 );
    ground_truth_partition.change_label_of_element( 6 , 0 );
    ground_truth_partition.change_label_of_element( 7 , 0 );
    ground_truth_partition.change_label_of_element( 8 , 0 );
    ground_truth_partition.change_label_of_element( 9 , 1 );
    ground_truth_partition.change_label_of_element( 10 , 0 );
    ground_truth_partition.change_label_of_element( 11 , 0 );
    ground_truth_partition.change_label_of_element( 12 , 0 );
    ground_truth_partition.change_label_of_element( 13 , 0 );
    ground_truth_partition.change_label_of_element( 14 , 1 );
    ground_truth_partition.change_label_of_element( 15 , 1 );
    ground_truth_partition.change_label_of_element( 16 , 0 );
    ground_truth_partition.change_label_of_element( 17 , 0 );
    ground_truth_partition.change_label_of_element( 18 , 1 );
    ground_truth_partition.change_label_of_element( 19 , 0 );
    ground_truth_partition.change_label_of_element( 20 , 1 );
    ground_truth_partition.change_label_of_element( 21 , 0 );
    ground_truth_partition.change_label_of_element( 22 , 1 );
    ground_truth_partition.change_label_of_element( 23 , 1 );
    ground_truth_partition.change_label_of_element( 24 , 1 );
    ground_truth_partition.change_label_of_element( 25 , 1 );
    ground_truth_partition.change_label_of_element( 26 , 1 );
    ground_truth_partition.change_label_of_element( 27 , 1 );
    ground_truth_partition.change_label_of_element( 28 , 1 );
    ground_truth_partition.change_label_of_element( 29 , 1 );
    ground_truth_partition.change_label_of_element( 30 , 1 );
    ground_truth_partition.change_label_of_element( 31 , 1 );
    ground_truth_partition.change_label_of_element( 32 , 1 );
    ground_truth_partition.change_label_of_element( 33 , 1 );
    std::cout << "ground_truth_partition = " << ground_truth_partition << std::endl;

    std::cout << "Thermalizing the meta_edm..." << std::endl;

    double meta_beta = 3.;
    std::cout << "meta_beta = " << meta_beta << std::endl;

    medm.linear_annealing( 1./1000. ,  meta_beta , 1000 );
    medm.thermalize( meta_beta , 10 , true );
    std::cout << medm << std::endl;

    std::cout << "Integrating the meta_edm..." << std::endl;

    // Ground-truth.
    RunningStats rs_NMI_gt;
    RunningStats rs_AMI_gt;
    // Spin-glass.
    RunningStats rs_NMI_sg;
    RunningStats rs_AMI_sg;
    medm.integrate( meta_beta , 5000 , ground_truth_partition , 100 , rs_NMI_gt , rs_AMI_gt , rs_NMI_sg , rs_AMI_sg , true );

}
