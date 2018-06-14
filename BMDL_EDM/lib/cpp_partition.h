#ifndef Included_cpp_partition
#define Included_cpp_partition

#include <cassert>
#include <cstddef>
#include <vector>
#include <set>
//#include <map>
#include <iostream>
#include <string>
#include <sstream>
//#include <algorithm>    // std::random_shuffle
//#include <ctime>        // std::time
//#include <cstdlib>     // std::rand, std::srand
//#include <limits>       // std::numeric_limits<double>::infinity()
//#include "cpp_static_graph.h"
#include "cpp_label_pool.h"
#include "to_string_patch.h"

class Partition {

    // This class implements an efficient administrator of:
    //  1. which is the set of elements associated to each label,
    //  2. which are the active labels.

    friend std::ostream& operator<<( std::ostream& os , Partition & npart ) {
           std::string sep = " ";
           os << "Partition{";
           for ( Partition::t_label_iterator l_it = npart.label_it() ; l_it ; ++l_it ) {
               int l = ( * l_it );
               //int cnt = npart.label_count( l );
               os << sep << l;
               //os << "~" << cnt;
               os << " :";
               for ( Partition::t_element_iterator i_it = npart.element_it( l ) ; i_it ; ++i_it ) {
                   int i = ( * i_it );
                   os << " " << i;
               }
               sep = " | ";
           }
           os << " }"; // << std::endl;       
           return os;
    }

    public:

        class t_label_iterator {
            friend class Partition;
            public:
                typedef t_label_iterator                  self_type;
                typedef int                               value_type;
                typedef std::forward_iterator_tag         iterator_category;
                t_label_iterator( const Partition & npart ) : _npart( ( & npart ) ) , _q( 0 ) {}
                t_label_iterator( const t_label_iterator & it ) : _npart( it._npart ) , _q( it._q ) {}
                //self_type operator=( const self_type & other ) { _map_it = other._map_it; return * this; }
                self_type operator++() {
                    // PREFIX
                    _q++;
                    return ( * this );                  
                }
                self_type operator++(int junk) {
                    // POSTFIX  
                    self_type it = ( * this ); 
                    _q++;
                    return it; 
                } 
                //value_type operator*() const { return _npart._wlp[ _q ]; }
                value_type operator*() const { return _npart -> _wlp[ _q ]; }
                //value_type * operator->() { return & ( * _map_it ).second; }
                //bool _same_npart( const self_type & it ) { return ( ( & _npart ) == ( & it._npart ) ); } 
                bool _same_npart( const self_type & it ) { return ( _npart == it._npart ); } 
                bool operator==( const self_type & rhs ) { return (     _same_npart( rhs ) and ( _q == rhs._q ) ); }
                bool operator!=( const self_type & rhs ) { return ( not _same_npart( rhs ) or  ( _q != rhs._q ) ); }
                operator bool() const { return _q < _npart -> _wlp.num_active_labels(); }
                // For Cython
                t_label_iterator() {}
                value_type deref() { return * ( * this ); }
                void inc() { ++( * this ); }
                bool keepgoing() { return ( * this ); }
            private:
                //const Partition &    _npart;
                const Partition *    _npart;
                int                  _q;
        };

        class t_element_iterator
        {
            friend class Partition;
            public:
                typedef t_element_iterator                   self_type;
                typedef int                               value_type;
                typedef std::forward_iterator_tag         iterator_category;
                t_element_iterator( const Partition & npart , int l ) : _npart( ( & npart ) ) , _l( l ) , _ini_i( -1 ) { 
                    //_i = _npart._begin[ _l ]; 
                    _i = _npart -> _begin[ _l ]; 
                }
                t_element_iterator( const t_element_iterator & it ) : _npart( it._npart ) , _l( it._l ) , _i( it._i ) , _ini_i( it._ini_i ) {}
                //self_type operator=( const self_type & other ) { 
                //    _npart = other._npart;
                //    _l     = other._l;
                //    _i     = other._i;
                //    _ini_i = other._ini_i;
                //    return ( * this ); 
                //}
                self_type operator++() {
                    // PREFIX
                    if ( _ini_i == -1 ) { _ini_i = _i; }
                    //_i = _npart._next[ _i ];
                    _i = _npart -> _next[ _i ];
                    return ( * this );                  
                }
                //self_type operator++(int junk) {
                //    // POSTFIX  
                //    self_type it = ( * this ); 
                //    _i = _npart._next[ _i ]; 
                //    return it; 
                // }
                value_type operator*() const { return _i; }
                //value_type * operator->() { return & ( * _map_it ).second; }
                //bool _same_npart( const self_type & it ) { return ( ( & _npart ) == ( & it._npart ) ); } 
                bool _same_npart( const self_type & it ) { return _npart == it._npart; } 
                bool operator==( const self_type & rhs ) { return (     _same_npart( rhs ) and ( _l == rhs._l ) and ( _i == rhs._i ) ); }
                bool operator!=( const self_type & rhs ) { return ( not _same_npart( rhs ) or  ( _l != rhs._l ) or  ( _i != rhs._i ) ); }
                operator bool() const { return _i != _ini_i; }
                // For Cython.
                t_element_iterator() {}
                value_type deref() { return * ( * this ); }
                void inc() { ++( * this ); }
                bool keepgoing() { return ( * this ); }
            private:
                //const Partition &        _npart;
                const Partition *        _npart;
                //const int                _l;
                int                      _l;
                int                      _i;
                int                      _ini_i;
        };    
        /*
        class const_iterator
        {
            friend class ExternalDegreeModel;
            public:
                typedef const_iterator     self_type;
                typedef std::set< int >    value_type;
                typedef std::map< int , value_type >::const_iterator map_iterator;
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
                int idx() { return ( * _map_it ).first; }
            private:
                map_iterator _map_it;
        }; 
        */

        Partition( int size ) : _wlp( size ) , _i_to_l( size ) , _begin( size ) , _prev( size ) , _next( size ) {

            // It initalizes to one label per element.

            //_wlp.resize( size );      // Weighted Label Pool.
            //_i_to_l.resize( size );   // i -> l
            //_begin.resize( size );    // l -> initial i
            //_prev.resize( size );     // i -> prev i
            //_next.resize( size );     // i -> next i

            for ( int i = 0 ; i < size ; i++ ) {
                // Initially, each element has its own label.
                int l = i; // for clarity?
                //int q = i;
                _i_to_l[ i ] = l;
                _begin[ l ]  = i;
                _prev[ i ]   = i; // Each double-linked list is a closed one. Since initially, each double-linked list is of size 1,
                _next[ i ]   = i; // then, the _prev and the _next of i are set to itslef.
                _wlp.inc_label( l );
            }

        }
        Partition( const Partition & other ) : _wlp( other._wlp ) , _i_to_l( other._i_to_l ) , _begin( other._begin ) , _prev( other._prev ) , _next( other._next ) {}
        ~Partition() {}
        
        Partition & operator=( const Partition & other ) {
            _wlp    = other._wlp;
            _i_to_l = other._i_to_l;
            _begin  = other._begin;
            _prev   = other._prev;
            _next   = other._next;
            return ( * this );
        }

        t_label_iterator label_it() const { return t_label_iterator( ( * this ) ); }
        t_element_iterator  element_it( int l ) const { return t_element_iterator( ( * this ) , l ); }

        int  size() const { return _i_to_l.size(); }
        int  num_active_labels() const { return _wlp.num_active_labels(); }
        int  label_count( int l ) const { return _wlp.label_count( l ); }
        int  total_count() const { return _wlp.total_count(); }

        bool is_label_active( int l ) const { return _wlp.is_label_active( l ); }
        int  label_of_element( int i ) const { return _i_to_l[ i ]; } 

        void change_label_of_element( int i , int new_l ) {

            int old_l    = _i_to_l[ i ];
            _i_to_l[ i ] = new_l;

            // before : k -> i -> q
            // after  : k -> q
            int k = _prev[ i ];
            int q = _next[ i ];
            _next[ k ] = q;
            _prev[ q ] = k;

            _wlp.dec_label( old_l );

            if ( _wlp.label_count( old_l ) == 0 ) { 
                _begin[ old_l ] = -1; 
            } else {
                _begin[ old_l ] = q; 
            }

            int j = _begin[ new_l ];
            if ( j == -1 ) {
                _prev[ i ] = i;
                _next[ i ] = i;
            } else {
                // before : k -> j -> q
                // after  : k -> j -> i -> q
                q = _next[ j ];
                _next[ j ] = i;
                _prev[ i ] = j;
                _prev[ q ] = i;
                _next[ i ] = q;
            }

            _wlp.inc_label( new_l );

            _begin[ new_l ] = i;

            check_consistency();

        }

        void check_consistency() {
            bool flag = true;
            int counti = 0;
            int countl = 0;
            std::set< int > seti;
            for ( t_label_iterator l_it = label_it() ; l_it ; ++l_it ) {
                int l = ( * l_it );
                int countil = 0;
                for ( t_element_iterator i_it = element_it( l ) ; i_it ; ++i_it ) {
                    int i = ( * i_it );
                    seti.insert( i );
                    counti++;
                    countil++;
                }
                countl++;
                if ( countil != label_count( l ) ) {
                    std::cout << "ERROR @ Partition.check_consistency : countil != label_count( l )" << std::endl;
                    std::cout << "l=" << l << std::endl;
                    std::cout << "countil=" << countil << std::endl;;
                    std::cout << "label_count( l )=" << label_count( l ) << std::endl;
                    flag = false;
                } 
            }
            if ( ( int ) seti.size() != size() ) {
                std::cout << "ERROR @ Partition.check_consistency : seti.size() != size()" << std::endl;
                std::cout << "seti.size()=" << seti.size() << std::endl;
                std::cout << "size()=" << size() << std::endl ;
                flag = false;
            } 
            if ( counti != size() ) {
                std::cout << "ERROR @ Partition.check_consistency : counti != size()" << std::endl;
                std::cout << "counti=" << counti << std::endl;
                std::cout << "size()=" << size() << std::endl;
                flag = false;
            }
            if ( total_count() != size() ) {
                std::cout << "ERROR @ Partition.check_consistency : total_count != size()" << std::endl;
                std::cout << "total_count()=" << total_count() << std::endl;
                std::cout << "size()=" << size() << std::endl;
                flag = false;
            }
            if ( countl != num_active_labels() ) {
                std::cout << "ERROR @ Partition.check_consistency : countl != num_active_labels()" << std::endl;
                std::cout << "countl=" << countl << std::endl;
                std::cout << "num_active_labels()=" << num_active_labels() << std::endl;
                flag = false;
            }
            if ( not flag ) {
                std::cout << "The inconsistency looks like..." << std::endl;
                std::cout << "Partition=" << ( * this ) << std::endl;
                _cout_double_linked_lists();
            }
            assert( flag );
        }

        void _cout_vec( std::vector< int > & v ) const {
            // Function for debugging purposes.
            std::cout << "[";
            for ( int i = 0 ; i < ( int ) v.size() ; i++ ) {
                std::cout << v[ i ] << ",\t";
            }
            std::cout << "]" << std::endl;
        }

        void _cout_double_linked_lists() const {
            for ( int i = 0 ; i < size() ; i++ ) {
                std::cout << "(" << _prev[ i ] << ";" << i << ";" << _next[ i ] << ") :: ";
            }
            for ( int l = 0 ; l < size() ; l++ ) {
                std::cout << "[l=" << l << ";i=" << _begin[ l ] << "] ";
            }
            std::cout << std::endl;
        }

        void cout_self() const {
            for ( int i = 0 ; i < size() ; i++ ) {
                std::cout << " i=" << i << " l=" << _i_to_l[ i ] << std::endl;
            }
            std::string sep = " ";
            std::cout << "Partition{";
            for ( Partition::t_label_iterator l_it = label_it() ; l_it ; ++l_it ) {
                int l = ( * l_it );
                std::cout << sep << l;
                std::cout << " :";
                for ( Partition::t_element_iterator i_it = element_it( l ) ; i_it ; ++i_it ) {
                    int i = ( * i_it );
                    std::cout << " " << i;
                }
                sep = " | ";
            }
            std::cout << " }" << std::endl;    
        }

    private:

        // i : element , l : label
        WeightedLabelPool     _wlp;
        std::vector< int >    _i_to_l;              // i -> l
        std::vector< int >    _begin;               // l -> initial i
        std::vector< int >    _prev;                // i -> prev i
        std::vector< int >    _next;                // i -> next i

};

#endif // Included_cpp_partition
