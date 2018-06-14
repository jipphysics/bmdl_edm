#ifndef Included_cpp_nhoa
#define Included_cpp_nhoa

#include <cstddef>
#include <set>
#include <map>
#include <vector>
//#include <array>        // std::array
#include <iostream>
#include <cassert>
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <functional>   // std::ref
#include "cpp_utils.h"
//#include "cpp_graph.h"
#include "cpp_static_graph.h"
#include "cpp_partition.h"
#include "cpp_edm.h"

class NhoaState {

    friend std::ostream & operator<<( std::ostream & os , NhoaState & ns ) {
        os << "NhoaState{" << std::endl; 
        os << "    " << ns._edm << std::endl;
        os << "    CNP={" << std::endl; 
        os << "        " << ns._cluster_npart << std::endl;
        os << "        CtoL{";
        std::string sep = " ";
        for ( t_cluster_iterator c_it = ns.cluster_it() ; c_it ; ++c_it ) {
            int c = ( * c_it );
            int l = ns._cluster_to_label[ c ];
            os << sep + "( c=" << c << " , l=" << l << " )";
            os << " : i=[";
            for ( t_clusterset_node_iterator i_it = ns.clusterset_node_it( c ) ; i_it ; ++i_it ) {
                int i = ( * i_it );
                os << " " << i;
            }
            os << " ]";
            sep = " | "; 
        }
        os << " }" << std::endl;
        os << "    }" << std::endl;
        os << "    E=" << ns.E() << " best_E=" << ns._best_E << std::endl; 
        os << "}";
        return os;
    }

    friend double run_nhoa( ExternalDegreeModel & edm , double beta  , long int loop_max , double err_tol , bool verbose , bool one_cluster_per_node , bool & failed_convergence ) {    

        // Description of the algorithm.
        // There are two node partitions: P and C. P denotes the node partition for the EDM. C denotes the node partition based on clusters.
        // C is a refinement of P. 
        // The algorithm begins with one part and one cluster per node. Alternativelly, if one_cluster_per_node = false, the algorithm begins with one cluster per label, again, based on the initial configuration.
        // Then, it runs over the following nested loops:
        //
        // Loop-1 : cluster-coarser loop. Based on the current labeling partition for the nodes, clusters are merged into a coarser version of them. Then, the control is passed into loop-2. The loop-1 continues until an iteration occurs in which no progress is made; there, the control is returned to the main flow of the algorithm for exiting.
        //
        // Loop-2 : shuffler loop. The list of clusters is shuffled, so the greedy optimization process performed by loop-3 and loop-4 is attempted in different ways. Thus, after shuffling, the control is passed to loop-3. The loop-2 continues until no progress is achieved, i.e. until an iteration occurs where no reduction of the current smallest value of effective_Hamiltonian_{beta P}(G) is achieved.
        //
        // Loop-3 : cluster-per-cluster loop. Here, the list of clusters is iterated using the cluster index ci. The idea is to greedely optimize effective_Hamiltonian_{beta P}(G) for each ci. So, for each ci, then the process continues entering Loop-4. The loop-3 continues until an iteration without progress occurs and the control is returned to loop-2.
        //
        // Loop-4 : other-cluster loop. Here, the list of clusters is iterated again, using the cluster index cj, so that the nodes in the cluster ci are re-labeled with the label associated to the cluster cj; this for each cj. If a cluster re-labeling decreases the effective_Hamiltonian_{beta P}(G) then, the change is kept. Otherwise, the change is discarded. In the end, the relabeling that leads to the smaller effective_Hamiltonian_{beta P}(G) is the one that survives. The loop continues until an iteration without progress occurs and the control is returned to loop-3.

        failed_convergence = false;

        // Check the trivial case...
        if ( edm.trivial_G() ) { return 0.; }

        // FOR TESTING.
        //std::cout << "Graph{\nNodes{";
        //for ( StaticGraph::t_node_iterator i_it = edm.G().node_it() ; i_it ; ++i_it ) {
        //    int i = ( * i_it );
        //    std::cout << " " << patch::to_string( i ) << ":" << patch::to_string( edm.G().degree( i ) );
        //    std::cout << ":k=" << patch::to_string( G._degree[ i ] );
        //}
        //std::cout << " }" << std::endl << "Edges{" << std::endl;
        //for ( StaticGraph::t_node_iterator i_it = edm.G().node_it() ; i_it ; ++i_it ) {
        //    int i = ( * i_it );
        //    for ( StaticGraph::t_nn_iterator j_it = edm.G().nn_it( i ) ; j_it ; j_it++ ) {
        //        int j = ( * j_it );
        //        std::cout << " (" << patch::to_string( i ) << "," << patch::to_string( j ) << ")";
        //    }
        //    std::cout << std::endl;
        //}
        //std::cout << "} }"; // << std::endl;

        // Create NhoaState
        NhoaState ns = NhoaState( edm , beta );
  
        // Init the random number generator.
        // http://stackoverflow.com/questions/15509270/does-stdmt19937-require-warmup
        // http://stackoverflow.com/questions/13166058/correctly-seeding-random-number-generator-mersenne-twister-c
        std::array< int , 624 > seed_data;
        std::random_device r;
        std::generate_n( seed_data.data() , seed_data.size() , std::ref( r ) );
        std::seed_seq seq( std::begin( seed_data ) , std::end( seed_data ) );
        std::mt19937 eng( seq );   
                                  
        // Loop delimiter...        
        long int loop = 0;
        
        // Register the best energy up-to now.
        ns._edm.update_quantities();    
        ns._best_E = ns.E();
        if ( verbose ) {
            std::cout << "# Initial ( loop , E ) = ( " << loop << " , " << ns._best_E << " ) ; edm = " << edm << std::endl;
        }
        

        // Lets regenerate the clusters, one per node, according to the labelings.
        bool one_per_node = one_cluster_per_node;
        ns.regenerate_clusters( one_per_node );
        
        // Level-1 loop : cluster-coarser loop.
        bool new_best_E_level1 = true;
        while ( new_best_E_level1 ) {
            new_best_E_level1 = false;

            // FOR TESTING.
            //std::cout << "@ run_nhoa : Entering level-1..." << std::endl;

            // Create the sequence of clusters.
            if ( not one_per_node ) {
                // ...so the relabeling should be one per cluster...
                ns.regenerate_clusters( one_per_node );
            }
            
            // We turn off the "one_per_node" flag so, in the future iterations inside loop-1, only the "one cluster per label" cluster regeneration is performed.
            one_per_node = false;
            // Then, create a vector of active clusters.
            std::vector< int > v_clusters( ns.num_clusters() );
            int ic = 0;
            for ( NhoaState::t_cluster_iterator c_it = ns.cluster_it() ; c_it; ++c_it ) { 
                int c = ( * c_it ); 
                v_clusters[ ic ] = c;
                ic++;
            }

            // Level-2 loop : shuffler loop.
            bool new_best_E_level2 = true;
            while ( new_best_E_level2 ) {
                new_best_E_level2 = false;

                // FOR TESTING.
                //std::cout << "@ run_nhoa : Entering level-2..." << std::endl;

                // Shuffle the sequence of clusters, using old C rand.
                // http://stackoverflow.com/questions/13459953/random-shuffle-not-really-random
                // http://stackoverflow.com/questions/13166058/correctly-seeding-random-number-generator-mersenne-twister-c
                std::shuffle( v_clusters.begin() , v_clusters.end() , eng );       

                // Level-3 loop : cluster-per-cluster loop.
                //for ic in xrange( v_clusters.shape[ 0 ] ) {
                for ( ic = 0 ; ic < ( int ) v_clusters.size() ; ic++ ) {

                    // FOR TESTING.
                    //std::cout << "@ run_nhoa : Entering level-3..." << std::endl;
                    //std::cout << "@ run_nhoa : 3-PRE : ns=" << ns << std::endl;
                    //ns.check_consistency();

                    // To mitigate numerical rounding errors.
                    ns._edm.update_quantities();

                    // Level-4 loop : other-cluster loop.
                    // Attempt to re-label cluster, loop.
                    //for jc in xrange( v_clusters.shape[ 0 ] ) {
                    for ( int jc = 0 ; jc < ( int ) v_clusters.size() ; jc++ ) {

                        // FOR TESTING.
                        //std::cout << "********************************************" << std::endl;
                        //std::cout << "@ run_nhoa : Entering level-4..." << std::endl;
                        //std::cout << "@ run_nhoa : PRE : ns=" << ns << std::endl;
                        //ns.check_consistency();

                        if ( ic == jc ) {
                            // No need to relabel a cluster into itself.

                            // FOR TESTING.
                            //std::cout << "@ run_nhoa : " << ic << "=ic==jc=" << jc << ", skipping." << std::endl;

                            continue;
                        }

                        // The cluster is relabeled.

                        int ci = v_clusters[ ic ];
                        int li = ns.cluster_to_label( ci );
                        //
                        int cj = v_clusters[ jc ];
                        int lj = ns.cluster_to_label( cj );

                        // FOR TESTING.
                        //std::cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
                        //std::cout << "@ run_nhoa : ci=" << ci;
                        //std::cout << " li=" << li; 
                        //std::cout << " ; cj=" << cj;
                        //std::cout << " lj=" << lj << std::endl;

                        // FOR TESTING.
                        //std::cout << "BEFORE attempt relabel_cluster ( loop , E ) = ( ";
                        //std::cout << loop;
                        //std::cout << " , ";
                        //std::cout << ns.E();
                        //std::cout << std::endl;

                        ns.relabel_cluster( ci , lj );
                            
                        // FOR TESTING.
                        //std::cout << "AFTER attempt relabel_cluster...";
                        //std::cout << " E = ";
                        //double current_E = ns.E();
                        //std::cout << current_E;
                        //std::cout << " )";
                        //std::cout << std::endl;
                        //std::cout << "Updating quantities...";
                        //ns._edm.update_quantities();
                        //std::cout << " E = ";
                        //std::cout << ns.E();
                        //std::cout << std::endl;
                        //if ( abs( current_E - ns.E() ) > 1. ) {
                        //    std::cout << "WARNING !!! : current_E - ns.E() = " << current_E - ns.E() << std::endl;
                        //}

                        // Check if the new labeling decreases the energy or
                        // not. We check this using certain numerical 
                        // tolerance E_err_tol.
                        //double E_err_tol = abs( err_tol * ns._best_E ) + err_tol;
                        //if ( ( ns.E() - ns._best_E ) < - E_err_tol ) {

                        // FOR TESTING.                                                
                        //std::cout << "@ run_nhoa : MID : ns=" << ns << std::endl;

                        //if ( ns.E() < ns._best_E ) {
                        //if ( abs( rel_diff( ns._best_E , ns.E() ) ) > err_tol ) { 
                        //    ns._edm.update_quantities();
                        //}

                        //if ( ns.E() < ns._best_E ) {                         
                        if ( rel_diff( ns._best_E , ns.E() ) > err_tol ) { // best_E - E > err_tol => best_E > E + err_tol => best_E > E => we need to update which is the best state and energy.
                                  
                            //ns._edm.update_quantities();
                            ns._best_E = ns.E();

                            new_best_E_level2 = true;
                            new_best_E_level1 = true;
                           
                            if ( verbose ) {
                                std::cout << "( loop , new_best_E ) = ( ";
                                std::cout << loop;
                                std::cout << " , ";
                                std::cout << ns._best_E;
                                std::cout << " )";
                                std::cout << std::endl;                              
                            }
                            
                        } else {

                            // If the energy did not decreased, we switch 
                            // back, into the previous node-labeling.                        

                            // FOR TESTING.
                            //std::cout << "@ run_nhoa : Relabelling back..." << std::endl;

                            ns.relabel_cluster( ci , li );

                        }

                        // FOR TESTING.
                        //std::cout << "@ run_nhoa : POST : ns=" << ns << std::endl;
                        //ns.check_consistency();                        

                        assert( ns.E() < ns._best_E + err_tol );

                        if ( loop >= loop_max - 1 ) {
                            std::cout << "WARNING : loop-max-stop @ loop = " << loop << std::endl;
                            failed_convergence = true;
                            return 100000000000000.0;
                        }    
                        loop += 1;
                            
                    } // loop jc
                } // loop ic
            } // loop level-2
        } // loop level-1

        if ( verbose ) {
            std::cout << "@ run_nhoa : End : eff_L = " << ns._edm.effective_L( beta ) << " eff_H = " << ns._edm.effective_Hamiltonian( beta ) << std::endl;
        }
    
        return ns.E();
            
    } // func run_nhoa()

    public:

        /*
        class iterator
        {
            friend class NhoaState;
            public:
                typedef iterator           self_type;
                typedef std::set< int >    value_type;
                typedef std::map< int , std::set< int > >::iterator    map_iterator;
                typedef std::forward_iterator_tag iterator_category;
                iterator( map_iterator map_it ) : _map_it( map_it ) { }
                iterator() { }                
                self_type operator=( const self_type & other ) { _map_it = other._map_it; return * this; }                                                
                self_type operator++()         { _map_it++; return *this;                  } // PREFIX
                self_type operator++(int junk) { self_type i = *this; _map_it++; return i; } // POSTFIX  
                value_type & operator*()  { return   ( * _map_it ).second; }
                value_type * operator->() { return & ( * _map_it ).second; }
                bool operator==( const self_type & rhs ) { return _map_it == rhs._map_it; }
                bool operator!=( const self_type & rhs ) { return _map_it != rhs._map_it; }
                int idx() { return ( * _map_it ).first; } 
            private:
                map_iterator _map_it;
        };    
        class const_iterator
        {
            friend class NhoaState;
            public:
                typedef const_iterator self_type;
                typedef std::set< int >    value_type;
                typedef std::map< int , std::set< int > >::const_iterator    map_iterator;
                typedef std::forward_iterator_tag iterator_category;
                const_iterator( map_iterator map_it ) : _map_it( map_it ) { }
                const_iterator() { }                
                self_type operator=( const self_type & other ) { _map_it = other._map_it; return * this; }                                
                self_type operator++()         { _map_it++; return *this;                  } // PREFIX
                self_type operator++(int junk) { self_type i = *this; _map_it++; return i; } // POSTFIX 
                const value_type & operator*()  const { return   ( * _map_it ).second; }
                const value_type * operator->() const { return & ( * _map_it ).second; }
                bool operator==( const self_type& rhs ) { return _map_it == rhs._map_it; }
                bool operator!=( const self_type& rhs ) { return _map_it != rhs._map_it; }
                int idx() const { return ( * _map_it ).first; }                 
            private:
                map_iterator _map_it;
        };       
        */

        /*
        iterator begin() { return _cluster_to_nodes.begin(); }
        iterator end()   { return _cluster_to_nodes.end();   }        
        const_iterator begin() const { return _cluster_to_nodes.begin(); }
        const_iterator end()   const { return _cluster_to_nodes.end();   }     

        std::set< int >::iterator node_begin( int c ) { return _cluster_to_nodes[ c ].begin(); }
        std::set< int >::iterator node_end( int c )   { return _cluster_to_nodes[ c ].end();   }
        */

        typedef Partition::t_label_iterator  t_cluster_iterator;
        typedef Partition::t_element_iterator   t_clusterset_node_iterator;

        t_cluster_iterator         cluster_it()                { return _cluster_npart.label_it(); }
        t_clusterset_node_iterator clusterset_node_it( int c ) { return _cluster_npart.element_it( c ); }

        NhoaState( ExternalDegreeModel & edm , double beta ) : _edm( edm ) , _beta( beta ) , _best_E( 0. ) , _cluster_to_label( edm.G().num_nodes() ) , _cluster_npart( edm.G().num_nodes() ) { 
             regenerate_clusters( true );            
            _best_E = E();
        }
        /*NhoaState & operator=( const NhoaState & other ) {
            _edm                   = other._edm;
            _beta                  = other._beta;
            _best_E                = other._best_E;
            _cluster_to_nodes      = other._cluster_to_nodes;
            _cluster_to_label      = other._cluster_to_label;
            return ( * this );
        }*/      
        ~NhoaState() {}

        ExternalDegreeModel & edm() const { return _edm; }

        double E() const { return _edm.effective_Hamiltonian( _beta ); }
        double best_E() const { return _best_E; }

        int  num_clusters() const { return _cluster_npart.num_active_labels(); }
        bool has_cluster( int c ) const { return _cluster_npart.is_label_active( c ); }
        int  cluster_to_label( int c ) const { assert( has_cluster( c ) ); return _cluster_to_label[ c ]; }

        void regenerate_clusters( bool one_per_node ) {
            if ( one_per_node ) {
                // Generat one cluster per node. Each cluster inherits the label
                // of its node. Therefore, if there are more than one node with
                // the same label, there will be more than one cluster with the same
                // label.

                // FOR TESTING.
                //std::cout << "Setting clusters; one per node..." << std::endl;

                for ( StaticGraph::t_node_iterator i_it = _edm.G().node_it() ; i_it ; ++i_it ) {
                    int i = ( * i_it );
                    int l = i;
                    int c = i;
                    _cluster_to_label[ c ] = l;
                    _cluster_npart.change_label_of_element( i , c );
                    _edm.set_node_label( i , l );
                }
            } else {
                // Generate one cluster per label. Therefore, different clusters have different labels, necessarily.

                // FOR TESTING.
                //std::cout << "Merging clusters; one per label..." << std::endl;

                for ( StaticGraph::t_node_iterator i_it = _edm.G().node_it() ; i_it ; ++i_it ) {
                    int i = ( * i_it );
                    int l = _edm.get_node_label( i );
                    int c = l;
                    _cluster_to_label[ c ] = l;
                    _cluster_npart.change_label_of_element( i , c );
                }
            }

            // FOR TESTING.
            //std::cout << "@ NhoaState.regenerate_clusters : _cluster_npart=" << _cluster_npart << std::endl;

        } 
        void relabel_cluster( int c , int l ) {

            // FOR TESTING.            
            //std::cout << "@ NhoaState.relabel_cluster : c=" << c << " l=" << l << std::endl;
            //std::cout << "@ NhoaState.relabel_cluster : _cluster_npart=" << _cluster_npart << std::endl;

            assert ( has_cluster( c ) );
            _cluster_to_label[ c ] = l;
            for ( t_clusterset_node_iterator i_it = clusterset_node_it( c ) ; i_it ; ++i_it ) {
                int i = ( * i_it );

                // FOR TESTING.
                //std::cout << "@ NhoaState.relabel_cluster : relabeling i=" << i << " with l=" << l << std::endl;
                //std::cout << "@ NhoaState.relabel_cluster : BEFORE E =" << E() << std::endl;

                _edm.set_node_label( i , l );

                // FOR TESTING.
                //std::cout << "@ NhoaState.relabel_cluster : AFTER E =" << E() << std::endl;

            }

        }
        //void initialize_labels();

        void _cout_vec( std::vector< int > & v ) {
            // Function for debugging purposes.
            std::cout << "[";
            for ( int i = 0 ; i < ( int ) v.size() ; i++ ) {
                std::cout << "\t" << v[ i ];
            }
            std::cout << "]" << std::endl;
        }

        void check_consistency() {
            bool flag = true;

            for ( Partition::t_label_iterator c_it = _cluster_npart.label_it() ; c_it ; ++c_it ) {
                int c = ( * c_it );
                int l = _cluster_to_label[ c ];
                for ( Partition::t_element_iterator i_it = _cluster_npart.element_it( c ) ; i_it ; ++i_it ) {
                    int i = ( * i_it );
                    if ( _edm.get_node_label( i ) != l ) {
                        std::cout << "ERROR @ NhoaState.check_consistency : _edm.get_node_label( i ) != l" << std::endl;
                        std::cout << "i=" << i << std::endl;
                        std::cout << "_edm.get_node_label( i )=" << _edm.get_node_label( i ) << std::endl;
                        std::cout << "l=" << l << std::endl;
                        flag = false;
                    }
                }
            }

            //if ( seti.size() != size() ) {
            //    std::cout << "ERROR @ Partition.check_consistency : seti.size() != size()" << std::endl;
            //    std::cout << "seti.size()=" << seti.size() << std::endl;
            //    std::cout << "size()=" << size() << std::endl ;
            //    flag = false;
            //} 
            assert( flag );
            if ( flag == true ) { flag = false; } // To avoid Cython to complain about unused variable.
        }


    private:    

        ExternalDegreeModel &                _edm;
        double                               _beta;
        double                               _best_E;
        std::vector< int >                   _cluster_to_label;
        Partition                            _cluster_npart; // Partition of nodes into clusters.

};   
            

double run_nhoa( ExternalDegreeModel & edm , double beta , long int loop_max , double err_tol , bool verbose , bool one_cluster_per_node , bool & failed_convergence );

double sample_nhoa( ExternalDegreeModel & edm , ExternalDegreeModel & best_edm , double beta , long int loop_max , double err_tol , bool verbose , int repetitions , bool & failed_convergence ) {
    double best_E = 10000000000000000.0;
    for ( int repetition = 0 ; repetition < repetitions ; repetition++ ) {
        bool _failed_convergence;
        double E = run_nhoa( edm , beta , loop_max , err_tol , verbose , true , _failed_convergence );
        if ( E < best_E ) {
            failed_convergence = _failed_convergence;
            best_E = E;
            best_edm = edm;
        }
    }
    return best_E;
}             
            
#endif // Included_cpp_nhoa
