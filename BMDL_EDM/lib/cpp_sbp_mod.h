#ifndef Included_cpp_sbp_mod
#define Included_cpp_sbp_mod

// Sparse Belief Propagation
// See
//     [1] Zhang, Pan, and Cristopher Moore. "Scalable detection of statistically significant communities and hierarchies, using message passing for modularity." Proceedings of the National Academy of Sciences 111.51 (2014): 18144-18149.
// and related work for an example of what is what I call SBP.

#include <cassert>
#include <cstddef>
#include <vector>
//#include <set>
//#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>      // std::random_shuffle , std::shuffle
#include <ctime>          // std::time
#include <cstdlib>        // std::rand, std::srand
#include <random>         // std::mt19937
#include <math.h>         // exp , log , sqrt
#include <cmath>          // std::abs , std::lgamma
//#include <limits>       // std::numeric_limits< double >::infinity()
#include "cpp_static_graph.h"
//#include "cpp_ltn.h"
//#include "cpp_edm.h"
//#include "cpp_partition.h"
//#include "cpp_info_theory.h"
#include "cpp_utils.h"
#include "cpp_random.h"
#include "cpp_sbp_rmdl.h"
//#include "cpp_runningstats.h"
#include "to_string_patch.h"

#include <iostream>
#include <fstream>

/*
LIST OF ALTERNATIVE IMPLEMENTATIONS

http://pgmpy.org/index.html
https://github.com/pgmpy/pgmpy
https://github.com/mbforbes/py-factorgraph
https://github.com/ilyakava/sumproduct

https://github.com/rdlester/pyfac
http://cs.brown.edu/courses/csci2420/lectures/
*/

/*
EXAMPLES : TO CREATE TESTS

http://helper.ipam.ucla.edu/publications/gss2013/gss2013_11344.pdf
*/

// *** For the pseudo-random number generator engine ***
//extern std::mt19937 _eng;

struct SBP_MOD {
    // This is the base-class for Graphical Models.

    const StaticGraph &                                    _I;
    int                                                    _num_labels;
    double                                                 _beta;
    std::vector< std::vector< double > >                   _n; // Messages n[ l ][ s ] where l=0,1,...,2E-1 and s=0,1,...,max_labels-1
    std::vector< double >                                  _n_old; // To compute max_err at each iteration.
    std::vector< std::vector< double > >                   _b; // Beliefs b[ i ][ s ] where i=0,1,...,N-1.
    std::vector< double >                                  _theta; // Local field theta[ s ]
    double                                                 _K; // K = 1. * 2E ; to improve speed.
    double                                                 _bexp; // bexp = exp( beta ) - 1. ; to improve speed.
    double                                                 _energy;
    double                                                 _entropy;
    double                                                 _paramagnetic_free_energy;
    double                                                 _paramagnetic_energy;
    double                                                 _paramagnetic_entropy;

    public:

    SBP_MOD( const StaticGraph & I , int num_labels_ , double beta ) : _I( I ) , _num_labels( num_labels_ ) , _beta( beta ) {
        // The constructor initializes the SBP_MOD graphical model in the paramagnetic phase.

        // Checks
        assert( _num_labels > 0 );
        assert( _beta > 0. );

        // Init useful constants
        _K = ( double ) 2 * _I.num_links();
        _bexp = std::exp( _beta ) - 1.;

        // Init _n
        _n.resize( 2 * num_factors() );
        for ( int l = 0 ; l < 2 * num_factors() ; l++ ) {
            _n[ l ].resize( num_labels() );
        }

        // Init _n_old
        _n_old.resize( num_labels() );

        // Init _b
        _b.resize( num_variables() );
        for ( int i = 0 ; i < num_variables() ; i++ ) {
            _b[ i ].resize( num_labels() );
        }

        // Init theta
        _theta.resize( num_labels() );

        // Init free_energy, energy and entropy to their paramagnetic values.
        set_paramagnetic_state();
        compute_thermostatistics();
    
        _paramagnetic_free_energy = free_energy();
        _paramagnetic_energy      = energy();
        _paramagnetic_entropy     = entropy();
    }
    SBP_MOD( SBP_MOD & other ) : _I( other._I ) , 
                                 _num_labels( other._num_labels ) , 
                                 _beta( other._beta ) , 
                                 _n( other._n ) ,
                                 _n_old( other._n_old ) ,  
                                 _b( other._b ) ,
                                 _theta( other._theta ) ,
                                 _K( other._K ) ,
                                 _bexp( other._bexp ) ,
                                 _energy( other._energy ) ,
                                 _entropy( other._entropy ) ,
                                 _paramagnetic_free_energy( other._paramagnetic_free_energy ) ,
                                 _paramagnetic_energy( other._paramagnetic_energy ) ,
                                 _paramagnetic_entropy( other._paramagnetic_entropy ) {}
    ~SBP_MOD() {}
    SBP_MOD & operator=( const SBP_MOD & rhs ) {
        //_I            = rhs._I;
        assert( _I == other._I );
        _num_labels   = rhs._num_labels;
        _beta         = rhs._beta;
        _n            = rhs._n;
        _n_old        = rhs._n_old;
        _b            = rhs._b;
        _theta        = rhs._theta;
        _K            = rhs._K;
        _bexp         = rhs._bexp;
        _energy       = rhs._energy;
        _entropy      = rhs._entropy;
        _paramagnetic_free_energy = rhs._paramagnetic_free_energy;
        _paramagnetic_energy      = rhs._paramagnetic_energy;
        _paramagnetic_entropy     = rhs._paramagnetic_entropy;
        return ( * this );
    }

    const StaticGraph & I() const { return _I; }
    const StaticGraph * I_ptr() const { return ( & _I ); }

    int num_variables() { return _I.num_nodes();     }
    int num_factors()   { return _I.num_links();     }
    int num_labels()    { return _num_labels;        }
    double beta()       { return _beta;              }
    double T()          { return 1. / _beta;         }

    void normalize_n( int l ) {
        double tmp = 0.;
        for ( int s = 0 ; s < num_labels() ; s++ )
            tmp += _n[ l ][ s ];
        tmp = 1. / tmp;
        for ( int s = 0 ; s < num_labels() ; s++ )
            _n[ l ][ s ] *= tmp;
        // FOR-TESTING
        //for ( int s = 0 ; s < num_labels() ; s++ )
        //    std::cout << "TEST @ SBP_MOD.normalize_n CP.1 : l=" << l << " s=" << s << " _n=" << _n[ l ][ s ] << std::endl;
    }

    void normalize_b( int i ) {
        double tmp = 0.;
        for ( int s = 0 ; s < num_labels() ; s++ )
            tmp += _b[ i ][ s ];
        tmp = 1. / tmp;
        for ( int s = 0 ; s < num_labels() ; s++ )
            _b[ i ][ s ] *= tmp;
    }

    void set_uniform_n() {
        std::uniform_real_distribution<> unit_dist( 0. , 1. );       
        for ( int l = 0 ; l < 2 * num_factors() ; l++ ) {
            for ( int s = 0 ; s < num_labels() ; s++ ) {
               _n[ l ][ s ] = 1.;
            }
            normalize_n( l );
        }
    }

    void set_uniform_b() {
        std::uniform_real_distribution<> unit_dist( 0. , 1. );       
        for ( int i = 0 ; i < num_variables() ; i++ ) {
            for ( int s = 0 ; s < num_labels() ; s++ ) {
               _b[ i ][ s ] = 1.;
            }
            normalize_b( i );
        }
    }

    void set_random_n() {
        std::uniform_real_distribution<> unit_dist( 0. , 1. );       
        for ( int l = 0 ; l < 2 * num_factors() ; l++ ) {
            for ( int s = 0 ; s < num_labels() ; s++ ) {
               _n[ l ][ s ] = unit_dist( ::_eng );
               // FOR-TESTING
               //std::cout << "TEST @ SBP_MOD.set_random_n : l=" << l << " s=" << s << " _n=" << _n[ l ][ s ] << std::endl;
            }
            normalize_n( l );
        }
    }

    void set_random_b() {
        std::uniform_real_distribution<> unit_dist( 0. , 1. );       
        for ( int i = 0 ; i < num_variables() ; i++ ) {
            for ( int s = 0 ; s < num_labels() ; s++ ) {
               _b[ i ][ s ] = unit_dist( ::_eng );
               // FOR-TESTING
               //std::cout << "TEST @ SBP_MOD.set_random_b : i=" << i << " s=" << s << " _b=" << _b[ i ][ s ] << std::endl;
            }
            normalize_b( i );
        }
    }

    void update_theta() {
        for ( int s = 0 ; s < num_labels() ; s++ ) {
            _theta[ s ] = 0.;
            for ( int i = 0 ; i < num_variables() ; i++ )
                _theta[ s ] += ( ( double ) _I.degree( i ) ) * _b[ i ][ s ];
        }
    }

    void set_paramagnetic_state() {
        set_uniform_n();
        set_uniform_b();
        update_theta();
        compute_thermostatistics();
    }

    double run_BP_step( StaticGraph::t_nn_iterator nn_it_ij , double dumping , int verbose ) {
        int i = nn_it_ij.i();
        int j = nn_it_ij.j();
        int l_ij = nn_it_ij.l();
        double k_i = ( double ) _I.degree( i );
        double k_j = ( double ) _I.degree( j );
        // Update n_{i<-j}
        for ( int s_j = 0 ; s_j < num_labels() ; s_j++ ) {
            _n_old[ s_j ] = _n[ l_ij ][ s_j ];
            double suma = - beta() * k_j * _theta[ s_j ] / _K;
            for ( StaticGraph::t_nn_iterator nn_it_jn = _I.nn_it( j ) ; nn_it_jn ; ++nn_it_jn ) {
                // FOR-TESTING
                //int _j = nn_it_jn.i();
                int n = nn_it_jn.j();
                int l_jn = nn_it_jn.l();
                if ( n != i ) {
                    suma += std::log( 1. + _bexp * _n[ l_jn ][ s_j ] );

                    // FOR-TESTING
                    // std::cout << "TEST @ SBP_MOD.run_BP_step CP.1.1 : l_jn=" << l_jn << " _j=" << _j << " n=" << n << " suma=" << suma << std::endl;
                    // std::cout << "TEST @ SBP_MOD.run_BP_step CP.1.2 : _bexp=" << _bexp << " _n[ l_jn ][ s_j ]=" << _n[ l_jn ][ s_j ] << " log(...)=" << std::log( 1. + _bexp * _n[ l_jn ][ s_j ] ) << std::endl;

                } // if n != i
            } // for nn_it_jn
            _n[ l_ij ][ s_j ] = ( 1. - dumping ) * _n_old[ s_j ] + dumping * std::exp( suma );

            // FOR-TESTING
            // std::cout << "TEST @ SBP_MOD.run_BP_step CP.2 : l_ij=" << l_ij << " i=" << i << " j=" << j << " s_j=" << s_j << " _n=" << _n[ l_ij ][ s_j ] << std::endl;

        } // for s_j
        normalize_n( l_ij );
        // Compute max_err
        double max_err = 0.;
        for ( int s_j = 0 ; s_j < num_labels() ; s_j++ )
            max_err = std::max( max_err , std::abs( _n[ l_ij ][ s_j ] - _n_old[ s_j ] ) );
        // Pre-update theta
        for ( int s_i = 0 ; s_i < num_labels() ; s_i++ )
            _theta[ s_i ] -= k_i * _b[ i ][ s_i ];
        // Update b_i
        for ( int s_i = 0 ; s_i < num_labels() ; s_i++ ) {
            double suma = - beta() * k_i * _theta[ s_i ] / _K;
            for ( StaticGraph::t_nn_iterator nn_it_in = _I.nn_it( i ) ; nn_it_in ; ++nn_it_in ) {
                int l_in = nn_it_in.l();
                suma += std::log( 1. + _bexp * _n[ l_in ][ s_i ] );
            }
            _b[ i ][ s_i ] = ( 1. - dumping ) * _b[ i ][ s_i ] + dumping * std::exp( suma );
        }
        normalize_b( i );
        // Finish-update theta
        for ( int s_i = 0 ; s_i < num_labels() ; s_i++ )
            _theta[ s_i ] += k_i * _b[ i ][ s_i ];
        // Return
        return max_err;
    }

    t_BP_summary run_BP_fast( int MC_steps , double err_tol , double dumping , double paramagnetic_tol , int verbose ) {

        t_BP_summary BP_summary;
        BP_summary.converged = false;
        BP_summary.max_err = 0.; // Infinity
        BP_summary.run_MC_steps = 0;
        //BP_summary.paramagnetic = false;

        set_random_n();
        set_random_b();
        update_theta();

        // Run the BP MC-steps
        std::vector< StaticGraph::t_nn_iterator > nn_it_pool( 2 * num_factors() );
        int l = 0;
        for ( int i = 0 ; i < _I.num_nodes() ; i++ )
            for ( StaticGraph::t_nn_iterator nn_it = _I.nn_it( i ) ; nn_it ; ++nn_it )
                nn_it_pool[ l++ ] = nn_it;
        //for ( int MC_step = 0 ; MC_step < MC_steps ; MC_step++ ) {
        for ( BP_summary.run_MC_steps = 0 ; BP_summary.run_MC_steps < MC_steps ; BP_summary.run_MC_steps++ ) {
            double annealing_dumping = dumping / std::log( 1. + ( double ) ( BP_summary.run_MC_steps + 1 ) );
            std::shuffle( nn_it_pool.begin() , nn_it_pool.end() , ::_eng );
            BP_summary.max_err = 0.;
            //for ( std::vector< StaticGraph::t_nn_iterator >::iterator l_pool_it = l_pool.begin() ; l_pool_it != l_pool.end() ; l_pool_it++ ) {
            for ( unsigned int l = 0 ; l < nn_it_pool.size() ; l++ ) {
                BP_summary.max_err = std::max( BP_summary.max_err , run_BP_step( nn_it_pool[ l ] , annealing_dumping , verbose ) );
            }
            if ( verbose > 1 )
                std::cout << std::string( 10 - verbose , ' ' ) << "@run_BP_fast: MC_step=" << BP_summary.run_MC_steps << " max_err=" << BP_summary.max_err << std::endl;
            // Complete summary
            if ( BP_summary.max_err < err_tol ) {
                BP_summary.converged = true;
                // Compute the thermostatistics
                compute_thermostatistics();
                //if ( abs( rel_diff( free_energy() , paramagnetic_free_energy() ) ) < paramagnetic_tol )
                //    BP_summary.paramagnetic = true;
                break;
            }
        } // MC_step

        return BP_summary;
    }

    std::vector< double > & b_i( int i ) { return _b[ i ]; }

    void compute_b_ij( int i , int j , std::vector< std::vector< double > > & b_ij ) {
        StaticGraph::t_nn_iterator nn_ij_it = _I.find_nn_it( i , j );
        StaticGraph::t_nn_iterator nn_ji_it = _I.find_nn_it( j , i );
        int l_ij = nn_ij_it.l();
        int l_ji = nn_ji_it.l();
        assert( nn_ij.i() == i );
        assert( nn_ij.j() == j );
        assert( nn_ji.i() == j );
        assert( nn_ji.j() == i );
        assert( b_ij.size() == num_labels() );
        for ( int s_i = 0 ; s_i < num_labels() ; s_i++ ) {
            assert( b_ij[ s_i ].size() == num_labels() );
            for ( int s_j = 0 ; s_j < num_labels() ; s_j++ ) {
                if ( s_i == s_j ) {
                    b_ij[ s_i ][ s_j ] = _bexp * _n[ l_ji ][ s_i ] * _n[ l_ij ][ s_j ];
                } else {
                    b_ij[ s_i ][ s_j ] = _n[ l_ji ][ s_i ] * _n[ l_ij ][ s_j ];
                }
            }
        }
        double suma = 0.;
        for ( int s_i = 0 ; s_i < num_labels() ; s_i++ )
            for ( int s_j = 0 ; s_j < num_labels() ; s_j++ )
                suma += b_ij[ s_i ][ s_j ];
        suma = 1. / suma;
        for ( int s_i = 0 ; s_i < num_labels() ; s_i++ )
            for ( int s_j = 0 ; s_j < num_labels() ; s_j++ )
                b_ij[ s_i ][ s_j ] *= suma;
    }

    int s_MAP( int i ) {
        double b;
        int _s_MAP = -1;
        double max_b = 0.;
        for ( int s = 0 ; s < num_labels() ; s++ ) {
            b = _b[ i ][ s ];

            // FOR-TESTING
            //std::cout << "TEST @ SBP_MOD.s_MAP : i=" << i << " s=" << s << " b=" << b << std::endl;

            if ( b > max_b ) {
                max_b = b;
                _s_MAP = s;
            }
        }
        return _s_MAP;
    }

    double H_i( int i , int s_i ) {
        return ( ( double ) _I.degree( i ) ) * _theta[ s_i ] / _K;
    }

    double H_ij( int i , int j , int s_i , int s_j ) {
        assert( _I.has_link( i , j ) );
        if ( s_i == s_j ) 
            return -1.;
        return 0.;
    }

    void compute_thermostatistics() {
        _energy = 0.;
        _entropy = 0.;
        // For b_i
        for ( int i = 0 ; i < num_variables() ; i++ ) {
             double k_i = ( double ) _I.degree( i );
             for ( int s = 0 ; s < num_labels() ; s++ ) {
                 double b = _b[ i ][ s ];
                 _energy += b * H_i( i , s );
                 _entropy -= ( 1. - k_i ) * xlnx( b );
             }
        }
        // For b_ij
        std::vector< std::vector< double > > b_ij;
        b_ij.resize( num_labels() );
        for ( int s = 0 ; s < num_labels() ; s++ ) b_ij[ s ].resize( num_labels() );
        for ( int i = 0 ; i < num_variables() ; i++ ) {
            for ( StaticGraph::t_nn_iterator nn_it = _I.nn_it( i ) ; nn_it ; ++nn_it ) {
                int j = nn_it.j();
                if ( i < j ) { // To count factors once, only.
                    compute_b_ij( i , j , b_ij );
                    for ( int s_i = 0 ; s_i < num_labels() ; s_i++ )
                        for ( int s_j = 0 ; s_j < num_labels() ; s_j++ ) {
                            double b = b_ij[ s_i ][ s_j ];
                            _energy += b * H_ij( i , j , s_i , s_j );
                            _entropy -= xlnx( b );
                        }
                } // if i < j
            }
        }
    } // thermostatistics

    double energy() { return _energy; }
    double entropy() { return _entropy; }
    double free_energy() { return energy() - T() * entropy(); }
    double paramagnetic_free_energy() { return _paramagnetic_free_energy; }
    double paramagnetic_energy() { return _paramagnetic_energy; }
    double paramagnetic_entropy() { return _paramagnetic_entropy; }

    double ln_Z() { return - beta() * free_energy(); }
    double hamiltonian( std::vector< int > & s ) {
        // This is $H_G(\mathcal{P})$, i.e. the meta hamiltonian.
        assert( s.size() == num_variables() );
        double H = 0.;
        for ( int i = 0 ; i < num_variables() ; i++ ) {
            H += H_i( i , s[ i ] );
        }        
        for ( int i = 0 ; i < num_variables() ; i++ ) {
            for ( StaticGraph::t_nn_iterator nn_it = _I.nn_it( i ) ; nn_it ; ++nn_it ) {
                int j = nn_it.j();
                if ( i < j ) { // To count factors (i.e. links) once, only.
                    H += H_ij( i , j , s[ i ] , s[ j ] );
                }
            }
        }      
        return H;
    }
    double RMDL( std::vector< int > & s ) { 
        // This is $L_{\beta',m}(\beta=0,m)= \beta' H_x(\beta=0,m) + ln Z_x(\beta')$, i.e. it is the meta RMDL.
        return beta() * hamiltonian( s ) + ln_Z();
    }

    friend std::ostream& operator<<( std::ostream& os , SBP_MOD & gm ) {
       std::string sep = " ";
       os << "SBP_MOD{" << std::endl;

       os << "    Edges" << std::endl;
       for ( int i = 0 ; i < gm.num_variables() ; i++ ) {
           for ( StaticGraph::t_nn_iterator nn_it = gm.I().nn_it( i ) ; nn_it ; nn_it++ ) {
               int i = nn_it.i();
               int j = nn_it.j();
               if ( i < j ) // To avoid counting edges twice.
                   os << "        " << "( " << i << " , " << j << " )" << std::endl;
           } // nn_it
       } // Iter i

       os << "    Messages" << std::endl;
       for ( int i = 0 ; i < gm.num_variables() ; i++ ) {
           for ( StaticGraph::t_nn_iterator nn_it = gm.I().nn_it( i ) ; nn_it ; nn_it++ ) {
               int i = nn_it.i();
               int j = nn_it.j();
               int l_ij = nn_it.l();
               for ( int s = 0 ; s < gm.num_labels() ; s++ )
                   os << "        n_{ " <<  i << " <- " << j << " }[ " << s << " ]=" << gm._n[ l_ij ][ s ] << std::endl;
           }
       }

       os << "    Beliefs" << std::endl;
       for ( int i = 0 ; i < gm.num_variables() ; i++ ) {
           for ( int s = 0 ; s < gm.num_labels() ; s++ )
               os << "        b_{ " <<  i << " }[ " << s << " ]=" << gm._b[ i ][ s ] << std::endl;
       }

       std::vector< std::vector< double > > b_ij;
       b_ij.resize( gm.num_labels() );
       for ( int s = 0 ; s < gm.num_labels() ; s++ ) b_ij[ s ].resize( gm.num_labels() );
       os << "    Belief pairs" << std::endl;
       for ( int i = 0 ; i < gm.num_variables() ; i++ ) {
           for ( StaticGraph::t_nn_iterator nn_it = gm.I().nn_it( i ) ; nn_it ; nn_it++ ) {
               int i = nn_it.i();
               int j = nn_it.j();
               //int l_ij = nn_it.l();
               if ( i < j ) {
                   //gm.compute_b_ij( i , j , b_ij );        
                   for ( int s_i = 0 ; s_i < gm.num_labels() ; s_i++ )
                       for ( int s_j = 0 ; s_j < gm.num_labels() ; s_j++ )
                           os << "        b_{ " <<  i << " " << j << " }[ " << s_i << " " << s_j << " ]=" << b_ij[ s_i ][ s_j ] << std::endl;
               }
           }
       }

       os << "    Community structure" << std::endl;
       for ( int i = 0 ; i < gm.num_variables() ; i++ )
       os << "        s_{MAP}[ " << i << " ]=" << gm.s_MAP( i ) << std::endl;

       os << "}"; // << std::endl;
       return os;
    }

};

std::ostream& operator<<( std::ostream& os , SBP_MOD & gm );

#endif // Included_cpp_sbp_mod
