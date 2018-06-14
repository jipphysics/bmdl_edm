#ifndef Included_cpp_edm
#define Included_cpp_edm

#include <cassert>
#include <cstddef>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
//#include <algorithm>      // std::random_shuffle
//#include <ctime>          // std::time
//#include <cstdlib>        // std::rand, std::srand
//#include <limits>         // std::numeric_limits<double>::infinity()
#include "cpp_partition.h"
#include "cpp_static_graph.h"
#include "to_string_patch.h"

class ExternalDegreeModel {

    friend std::ostream & operator<<( std::ostream & os , ExternalDegreeModel & edm ) {
        std::string sep = " ";
        os << "EDM{";
        for ( ExternalDegreeModel::t_label_iterator l_it = edm.label_it() ; l_it ; ++l_it ) {
            int l = ( * l_it );
            //int c = edm.label_count( l );
            //os << sep << l << "~" << c << " :";
            os << sep << l << " :";
            for ( ExternalDegreeModel::t_labelset_node_iterator i_it = edm.labelset_node_it( l ) ; i_it ; ++i_it ) {
                int i = ( * i_it );
                os << " " << patch::to_string( i );
            }
            sep = " | ";
        }
        os << " :: H_0 = " << patch::to_string( edm.effective_Hamiltonian_ord0() );
        os << " , H_1 = " << patch::to_string( edm.effective_Hamiltonian_ord1() );
        os << " }"; // << std::endl;       
        return os;
    }

    public:
  
        typedef Partition::t_label_iterator          t_label_iterator;
        typedef Partition::t_element_iterator        t_labelset_node_iterator;

        t_label_iterator label_it() { return t_label_iterator( _npart ); }     
        t_labelset_node_iterator labelset_node_it( int l ) { return t_labelset_node_iterator( _npart , l ); }

        ExternalDegreeModel( StaticGraph & G ) : _G( G ) , _npart( G.num_nodes() ) , _label_to_kappa( G.num_nodes() ) , _num_intra_links( 0 ) , _twice_avrg_num_intra_links( 0. ) , _effective_Hamiltonian_ord1( 0. ) , _total_kappa( 0. ) , _ln_modA( 0. ) {

            // Check Partition.
            assert( _npart.num_active_labels() == _G.num_nodes() );

            // Initialize the constants.
            //_total_kappa = 0.;
            for ( StaticGraph::t_node_iterator i_it = _G.node_it() ; i_it ; ++i_it ) {
                int i = ( * i_it );
                _total_kappa += ( double ) degree( i );
            }

            // Compute estiamte for ln |A|. This is also a constant, but we compute it separatelly since it is more complicate.
            // TODO
            /*
            double M = ( double ) _G.num_links();
            double N = ( double ) _G.num_nodes();
            _ln_modA = M * ( std::log( M ) - 1. );
            RunningStats row_k;
            RunningStats col_k;
            for ( int i = 0 ; i < _H.num_nodes() ; i++ ) {
                double k = _H.row_degree( i );
                _ln_modA -= std::lgamma( k + 1. ); // ln gamma( x + 1 ) = ln( x! )
                row_k.insert( k );
            }
            for ( int i = 0 ; i < _H.num_cols() ; i++ ) {
                double k = _H.col_degree( i );
                _ln_modA -= std::lgamma( k + 1. ); // ln gamma( x + 1 ) = ln( x! )
                col_k.insert( k );
            }
            _ln_modA -= .5 * N_row * N_col * ( row_k.second_moment() - row_k.mean() ) * ( col_k.second_moment() - col_k.mean() ) / ( M * M );
            */
            _ln_modA = 0.;

            // Initialize the non-constant quantities.
            update_quantities();

        }
        ExternalDegreeModel( const ExternalDegreeModel & edm ) : _G( edm._G ) , _npart( edm._npart ) , _label_to_kappa( edm._label_to_kappa ) , _num_intra_links( edm._num_intra_links ) , _twice_avrg_num_intra_links( edm._twice_avrg_num_intra_links ) , _effective_Hamiltonian_ord1( edm._effective_Hamiltonian_ord1 ) , _total_kappa( edm._total_kappa ) , _ln_modA( edm._ln_modA ) {}
        ~ExternalDegreeModel() {}      

        ExternalDegreeModel & operator=( const ExternalDegreeModel & other ) {
            //_G                          = other._G;
            assert( _G == other._G );
            _npart                      = other._npart;
            _label_to_kappa             = other._label_to_kappa;
            _num_intra_links            = other._num_intra_links;
            _twice_avrg_num_intra_links = other._twice_avrg_num_intra_links;
            _effective_Hamiltonian_ord1 = other._effective_Hamiltonian_ord1;
            _total_kappa                = other._total_kappa;
            _ln_modA                    = other._ln_modA;
            return ( * this );
        }

        const StaticGraph & G() const { return _G; }
        const StaticGraph * G_ptr() const { return ( & _G ); }

        bool trivial_G() { return ( _G.num_nodes() <= 1 or _G.num_links() == 0 ); }

        void update_quantities() {

            // Set to zero different quantities...
            _num_intra_links = 0;
            _twice_avrg_num_intra_links = 0.;
            _effective_Hamiltonian_ord1 = 0.;
            for( t_label_iterator l_it = label_it() ; l_it ; ++l_it  ) {
                int l = ( * l_it );
                _label_to_kappa[ l ] = 0.;
            }

            if ( not trivial_G() ) {

                // Update _label_to_kappa and _num_intra_links.
                for ( StaticGraph::t_node_iterator i_it = _G.node_it() ; i_it ; ++i_it ) {
                    int i = ( * i_it );
                    int l = get_node_label( i );
                    _label_to_kappa[ l ] += degree( i );
                    for ( StaticGraph::t_nn_iterator j_it = _G.nn_it( i ) ; j_it ; ++j_it ) {
                        int j = ( * j_it );
                        if ( i < j ) { // Otherwise, intra-module links are counted twice.
                            if ( l == get_node_label( j ) ) { _num_intra_links++; }
                        }            
                    }                
                }

                // At this stage, _label_to_kappa and _num_intra_links are ready.
                // Update _twice_avrg_num_intra_links and _effective_Hamiltonian_ord1.
                for ( StaticGraph::t_node_iterator i_it = _G.node_it() ; i_it ; ++i_it ) {
                    int i = ( * i_it );
                    _twice_avrg_num_intra_links += avrg_internal_degree( i );
                    _effective_Hamiltonian_ord1 += effective_Hamiltonian_ord1_i( i );
                }      
      
            }
            
        }

        int get_node_label( int i ) const { assert( _G.has_node( i ) ); return _npart.label_of_element( i ); }
        //
		void set_node_label( int i , int new_l ) {
		     /*
		     The quantities: 
		         _kappa_l
		         _num_intra_links
		         _twice_avrg_num_intra_links
		         _effective_Hamiltonian_ord1
		    are updated in this function. The state-variables:
		         _npart
		    is updated, also.
		    */

            // FOR TESTING.
            //std::cout << "@ EDM.set_node_label : i=" << i << " new_l=" << new_l << std::endl;

		    assert( _G.has_node( i ) );
		            
            // FOR TESTING.
            //std::cout << "@ ExternalDegreeModel.set_node_label BEFORE ANYTHING : j_1() = " << j_1() << std::endl;

		    // Check for the trivial case...
		    if ( trivial_G() ) { return; }
		    
		    int old_l = get_node_label( i );
		    if ( old_l == new_l ) {
		        // If new label equals old label then, nothing should be done.
		        return;
		    }        
		            
		    // Pre-update:
		    //     _twice_avrg_num_intra_links
		    //     _effective_Hamiltonian_ord1
		    for ( t_labelset_node_iterator j_it = labelset_node_it( old_l ) ; j_it ; ++j_it ) {
		        int j = ( * j_it );
		        _twice_avrg_num_intra_links -= avrg_internal_degree( j );
		        _effective_Hamiltonian_ord1 -= effective_Hamiltonian_ord1_i( j );            
		    }        
		    if ( has_label( new_l ) ) {
     		    for ( t_labelset_node_iterator j_it = labelset_node_it( new_l ) ; j_it ; ++j_it ) {
		            int j = ( * j_it );
		            _twice_avrg_num_intra_links -= avrg_internal_degree( j );
		            _effective_Hamiltonian_ord1 -= effective_Hamiltonian_ord1_i( j );            
		        }        
		    }
		      
		    // Pre-update:
		    //    _label_to_kappa    
		    _label_to_kappa[ old_l ] -= degree( i );          

            // FOR TESTING.
            //std::cout << "@ ExternalDegreeModel.set_node_label AFTER PRE-UPDATE : j_1() = " << j_1() << std::endl;
            //std::cout << "@ ExternalDegreeModel.set_node_label : degree( i ) = " << degree( i ) << std::endl;

		    // Relabel i.
            _npart.change_label_of_element( i , new_l ); 		    

            // FOR TESTING.
            //std::cout << "@ ExternalDegreeModel.set_node_label AFTER change_label_of_element : j_1() = " << j_1() << std::endl;

		    // Post-update:
		    //    _label_to_kappa     
		    _label_to_kappa[ new_l ] += degree( i );

		    // Update the number of intra-links.
		    //for (  std::set< int >::iterator j_it = _G.nn_begin( i ) ; j_it != _G.nn_end( i ) ; j_it++ ) {
            for ( StaticGraph::t_nn_iterator j_it = _G.nn_it( i ) ; j_it ; ++j_it ) {
		        /* For each j, there are four cases to consider:
		            i)   old_l == l_j & new_l == l_j
		            ii)  old_l != l_j & new_l == l_j
		            iii) old_l == l_j & new_l != l_j
		            iv)  old_l != l_j & new_l != l_j
		        Case i) is impossible because old_l != new_l by construction. This is ensured above where, if old_l == new_l then returns.
		        Case ii) means that the link (i,j) was inter-link and became intra-link.
		        Case iii) means that the link (i,j) was intra-link and became inter-link.
		        Case iv) means that the link (i,j) was inter-link and remained inter-link.
		        Only cases ii) and iii) affects _num_intra_links. 
		        In case ii) the _num_intra_links increases by one.
		        In case iii) the _num_intra_links decreases by one.
		        When checking which condition is satisfied, there is no need to check for both, old_l and new_l. This is because old_l != new_l by construction.
		        */
                 
		        int j = ( * j_it );
		        int l_j = get_node_label( j ); //_node_to_label[ j ];

                // FOR TESTING
                //std::cout << "@ ExternalDegreeModel.set_node_label j-loop : j = " << j << " l_j = " << l_j << std::endl;

		        // Case ii)  : inter-link becomes intra-link.
		        if ( new_l == l_j ) { 
		            _num_intra_links++;
		        }
		        // Case iii) : intra-link becomes inter-link. 
		        if ( old_l == l_j ) { 
		            _num_intra_links--; 
		        }
		    }

            // FOR TESTING.
            //std::cout << "@ ExternalDegreeModel.set_node_label AFTER updating the number of intra-links : j_1() = " << j_1() << std::endl;
	  
		    // Post-update:
		    //     _twice_avrg_num_intra_links
		    //     _effective_Hamiltonian_ord1
		    if ( has_label( old_l ) ) {
		        //for ( std::set< int >::iterator j_it = nodes_begin( old_l ) ; j_it != nodes_end( old_l ) ; j_it++ ) {
     		    for ( t_labelset_node_iterator j_it = labelset_node_it( old_l ) ; j_it ; ++j_it ) {
		            int j = ( * j_it );
		            _twice_avrg_num_intra_links += avrg_internal_degree( j );
		            _effective_Hamiltonian_ord1 += effective_Hamiltonian_ord1_i( j );            
		        }        
		    }
		    //for ( std::set< int >::iterator j_it = nodes_begin( new_l ) ; j_it != nodes_end( new_l ) ; j_it++ ) {
     		for ( t_labelset_node_iterator j_it = labelset_node_it( new_l ) ; j_it ; ++j_it ) {
		        int j = ( * j_it );
		        _twice_avrg_num_intra_links += avrg_internal_degree( j );
		        _effective_Hamiltonian_ord1 += effective_Hamiltonian_ord1_i( j );            
		    }        

            // FOR TESTING.
            //std::cout << "@ ExternalDegreeModel.set_node_label AFTER POST-UPDATE : j_1() = " << j_1() << std::endl;

		}
 
        bool has_label( int l ) const { return _npart.is_label_active( l ); }
        int num_active_labels() const { return _npart.num_active_labels(); }

        // In the main-text, kappa is denoted as 2E.
        double total_kappa() const { return _total_kappa; }
        // In the main-text, kappa_p is denoted as 2E_p.
        double kappa( int l ) const { 
            if ( has_label( l ) ) { 
                return _label_to_kappa[ l ]; 
            } 
            return 0.;  
        }

        int num_intra_links() const { return _num_intra_links;                  }
        int num_inter_links() const { return _G.num_links() - _num_intra_links; }

        double avrg_num_intra_links() const { return _twice_avrg_num_intra_links / 2.; }
        double avrg_num_inter_links() const { return ( ( double ) _G.num_links() ) - avrg_num_intra_links(); }

        double degree( int i ) const { return ( double ) _G.degree( i ); }

        double external_degree( int i ) const {
            assert( _G.has_node( i ) ); 
            int suma = 0;
            int l = get_node_label( i );
            for ( StaticGraph::t_nn_iterator j_it = _G.nn_it( i ) ; j_it ; ++j_it ) {
                if ( l != get_node_label( ( * j_it ) ) ) { suma += 1; }
            }
            return ( double ) suma;
        }
        double internal_degree( int i ) const {       
            return degree( i ) - external_degree( i );        
        }

        double avrg_external_degree( int i ) const {
            assert( _G.has_node( i ) );
            int l          = get_node_label( i );        
            double kappa_l = kappa( l );
            double k_i     = degree( i );
            return k_i * ( _total_kappa - kappa_l ) / ( _total_kappa - k_i );
        }
        double avrg_internal_degree( int i ) const { 
            assert( _G.has_node( i ) );
            int l          = get_node_label( i );        
            double kappa_l = kappa( l );
            double k_i     = degree( i );
            return k_i * ( kappa_l - k_i ) / ( _total_kappa - k_i );
        }

        double effective_Hamiltonian_ord1_i( int i ) const {
            int    l       = get_node_label( i );
            double k_i     = degree( i );
            double kappa_l = _label_to_kappa[ l ];
            double tmp     = ( _total_kappa - k_i );
            return .5 * k_i * ( _total_kappa - kappa_l ) * ( kappa_l - k_i ) / ( tmp * tmp );
        }

        double Hamiltonian() const { return ( 2. * ( double ) num_inter_links() ); }

        double effective_Hamiltonian_ord0() const { return - 2. * ( ( double ) num_intra_links() ) + _twice_avrg_num_intra_links; }
        double effective_Hamiltonian_ord1() const { return _effective_Hamiltonian_ord1; }

        double effective_Hamiltonian( double beta ) const { 
            assert( beta >= 0. ); 
            return effective_Hamiltonian_ord0() + beta * effective_Hamiltonian_ord1(); 
        }
        double effective_L( double beta ) const { return beta * effective_Hamiltonian( beta ); }

        double ln_modA() const { return _ln_modA; }

        double mdl( double beta ) const { return _ln_modA + effective_L( beta ); }

        Partition & node_partition() { return _npart; }
        Partition * node_partition_ptr() { return ( & _npart ); }

        void set_one_label_per_node() {
            // To set the trivial partition of one label per node.
            for ( StaticGraph::t_node_iterator i_it = _G.node_it() ; i_it ; ++i_it ) {
                int i = ( * i_it );
                set_node_label( i , i );
            }               
        }
        void set_one_label_all_nodes() {
            // To set the trivial partition of one label for all nodes.
            for ( StaticGraph::t_node_iterator i_it = _G.node_it() ; i_it ; ++i_it ) {
                int i = ( * i_it );
                set_node_label( i , 0 );
            }               
        }

    private:

        const StaticGraph &                  _G;
        //StaticGraph &                        _G;

        Partition                            _npart;
        std::vector< int >                   _label_to_kappa;

        int                                  _num_intra_links;
        double                               _twice_avrg_num_intra_links;
        double                               _effective_Hamiltonian_ord1;      
        double                               _total_kappa;        
        double                               _ln_modA;

};

//std::ostream & operator<<( std::ostream& os , ExternalDegreeModel & edm );

#endif // Included_cpp_edm
