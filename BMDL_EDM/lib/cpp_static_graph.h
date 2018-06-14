#ifndef Included_cpp_static_graph
#define Included_cpp_static_graph

#include <cassert>
#include <cstddef>
#include <vector>
#include <set>
//#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>      // ifstream
#include <algorithm>    // std::random_shuffle , std::shuffle (uses a random num. generator, e.g. std::mt19937) , std::max
#include <random>       // std::mt19937
#include <functional>   // std::ref
#include "to_string_patch.h"

//int _cmp_pairs( const std::pair< int , int > & p1 , const std::pair< int , int > & p2 ) {
//    if ( p1.first == p2.first ) { 
//        if ( p1.second == p2.second ) { return 0; }
//        else if ( p1.second < p2.second ) { return 1; }
//        return -1;
//    } else if ( p1.first < p2.first ) { return 1; }
//    return -1;
//}
   
class StaticGraph {

    friend std::ostream& operator<<( std::ostream& os , StaticGraph & G ) {    
        os << "Graph{\nNodes{";
        // Print nodes.
        for ( StaticGraph::t_node_iterator i_it = G.node_it() ; i_it ; ++i_it ) {
            int i = ( * i_it );
            os << " " << patch::to_string( i ) << ":" << patch::to_string( G.degree( i ) );
            //os << ":k=" << patch::to_string( G._degree[ i ] );
        }
        os << " }" << std::endl << "Edges{" << std::endl;
        // Print edges.              
        for ( StaticGraph::t_node_iterator i_it = G.node_it() ; i_it ; ++i_it ) {
            int i = ( * i_it );
            for ( StaticGraph::t_nn_iterator j_it = G.nn_it( i ) ; j_it ; j_it++ ) {
                int j = ( * j_it );
                //if ( i < j ) { os << " (" << patch::to_string( i ) << "," << patch::to_string( j ) << ")"; }
                os << " (" << patch::to_string( i ) << "," << patch::to_string( j ) << ")";
            }
            os << std::endl;
        }
        os << "} }"; // << std::endl;
        return os;
    }   

    public:

        class t_node_iterator
        {
            // To iterate over nodes.
            friend class StaticGraph;
            public:
                typedef t_node_iterator              self_type;
                typedef int                          value_type;
                typedef std::forward_iterator_tag    iterator_category;
                t_node_iterator( const StaticGraph & G ) : _G( G ) , _i( 0 ) {}
                //t_node_iterator( t_node_iterator & i ) : _G( i._G ) , _i( i._i ) {}
                //self_type operator=( const int i ) { assert( _G.has_node( i ) ) ; _i = i; return ( * this ); }
                //self_type operator=( const self_type & other ) { _i = other._i; return ( * this ); }                                
                self_type operator++()         { _i++; return ( * this );                  } // PREFIX
                //self_type operator++(int junk) { self_type i = ( * this ); _i++; return i; } // POSTFIX  
                value_type & operator*() { return _i; }
                //value_type operator*() { return _i; }
                //value_type * operator->() { return & _i; }
                bool _same_G( const self_type & i ) { return ( ( & _G ) == ( & i._G ) ); } 
                bool operator==( const self_type & rhs ) { assert( _same_G( rhs ) ); return _i == rhs._i; }
                bool operator!=( const self_type & rhs ) { assert( _same_G( rhs ) ); return _i != rhs._i; }
                operator bool() const { return _i < _G._num_nodes; }
            private:
                const StaticGraph &    _G;
                int                    _i;
        };    
        /*
        class t_const_node_iterator
        {
            friend class StaticGraph;
            friend class t_node_iterator;
            public:
                typedef t_const_node_iterator        self_type;
                typedef int                          value_type;
                typedef std::forward_iterator_tag    iterator_category;
                t_const_node_iterator( const StaticGraph & G ) : _G( G ) , _i( 0 ) {}
                t_const_node_iterator( const t_node_iterator & i ) : _G( i._G ) , _i( i._i ) {}
                t_const_node_iterator( const t_const_node_iterator & i ) : _G( i._G ) , _i( i._i ) {}
                //self_type operator=( const int i ) const { assert( _G.has_node( i ) ) ; _i = i; return ( * this ); }
                //self_type operator=( const self_type & other ) const { _G = other._G; _i = other._i; return ( * this ); }
                self_type operator++()         { _i++; return ( * this );                  } // PREFIX
                //self_type operator++(int junk) { self_type i = ( * this ); _i++; return i; } // POSTFIX 
                const value_type & operator*() const { return _i; }
                //const value_type * operator->() const { return & _i; }
                bool _same_G( const self_type & i ) { return ( ( & _G ) == ( & i._G ) ); } 
                bool operator==( const self_type& rhs ) { assert( _same_G( rhs ) ); return _i == rhs._i; }
                bool operator!=( const self_type& rhs ) { assert( _same_G( rhs ) ); return _i != rhs._i; }
                operator bool() const { return _i < _G._num_nodes; }
            private:
                const StaticGraph &    _G;
                int                    _i;
        };
        */

        class t_nn_iterator
        {
            // To iterate over nodes.
            friend class StaticGraph;
            public:
                typedef t_nn_iterator                self_type;
                typedef int                          value_type;
                typedef std::forward_iterator_tag    iterator_category;

                t_nn_iterator() : _G_ptr( NULL ) , _i( 0 ) , _l( 0 ) {}
                t_nn_iterator( const StaticGraph & G , int i ) : _G_ptr( & G ) , _i( i ) , _l( 0 ) { 
                    assert( _G_ptr -> has_node( i ) ); 
                    _l = _G_ptr -> _begin[ _i ]; 
                }
                t_nn_iterator( const t_nn_iterator & n ) : _G_ptr( n._G_ptr ) , _i( n._i ) , _l( n._l ) {}

                t_nn_iterator & operator=( const t_nn_iterator & other ) {
                    _G_ptr = other._G_ptr;
                    _i     = other._i;
                    _l     = other._l;
                    return ( * this );
                }

                self_type operator++()         { _l++; return ( * this );                  } // PREFIX
                self_type operator++(int junk) { self_type n = ( * this ); _l++; return n; } // POSTFIX  

                const value_type & operator*() const { return _G_ptr -> _nn[ _l ]; }
                //value_type * operator->() { return & _i; }

                bool _same_G( const self_type & n ) { return ( _G_ptr == n._G_ptr ); } 
                bool operator==( const self_type & rhs ) { return ( _same_G( rhs ) and ( _i == rhs._i ) and ( _l == rhs._l ) ); }
                bool operator!=( const self_type & rhs ) { return ( ( not _same_G( rhs ) ) or ( _i != rhs._i ) or ( _l != rhs._l ) ); }
                bool operator<( const self_type & rhs )  { assert( _same_G( rhs ) ); return ( _l <  rhs._l ); }
                bool operator>( const self_type & rhs )  { assert( _same_G( rhs ) ); return ( _l >  rhs._l ); }
                bool operator<=( const self_type & rhs ) { assert( _same_G( rhs ) ); return ( _l <= rhs._l ); }
                bool operator>=( const self_type & rhs ) { assert( _same_G( rhs ) ); return ( _l >= rhs._l ); }
                operator bool() const { return _l < _G_ptr -> _begin[ _i ] + _G_ptr -> _degree[ _i ]; }
                value_type j_th( const int j ) { 
                    assert( j >= 0 ); 
                    assert( j < _G_ptr -> _degree[ _i ] ); 
                    return _G_ptr -> _nn[ _G_ptr -> _begin[ _i ] + j ]; 
                }
                int i() { return _i; }
                int j() { return ( * ( * this ) ); }
                int l() { return _l; }
                bool keepgoing() { return ( * this ); } // Useful for Cython.
                void inc() { ++( * this ); } // Useful for Cython.

            private:
                //const StaticGraph &    _G;
                const StaticGraph *    _G_ptr;
                //const int              _i;
                int                    _i;
                int                    _l;
        };    
        /*
        class t_const_nn_iterator
        {
            friend class StaticGraph;
            friend class t_nn_iterator;
            public:
                typedef t_const_nn_iterator          self_type;
                typedef int                          value_type;
                typedef std::forward_iterator_tag    iterator_category;
                t_const_nn_iterator( const StaticGraph & G , const int i ) : _G( G ) , _i( i ) , _l( _G._begin[ _i ] ) {}
                t_const_nn_iterator( const t_nn_iterator & n ) : _G( n._G ) , _i ( n._i ) , _l( n._l ) {}
                t_const_nn_iterator( const t_const_nn_iterator & n ) : _G( n._G ) , _i ( n._i ) , _l( n._l ) {}
                //self_type operator=( const self_type & other ) { _G = other._G; _i = other._i; _l = other._l; return ( * this ); }
                self_type operator++()         { _l++; return ( * this );                  } // PREFIX
                self_type operator++(int junk) { self_type n = ( * this ); _l++; return n; } // POSTFIX 
                const value_type & operator*() const { return _G._nn[ _l ]; }
                //const value_type operator*() const { return _G._nn[ _l ]; }
                //const value_type * operator->() const { return & _i; }
                bool _same_G( const self_type & n ) { return ( ( & _G ) == ( & n._G ) );  } 
                bool operator==( const self_type & rhs ) { return ( _same_G( rhs ) and ( _i == rhs._i ) and ( _l == rhs._l ) ); }
                bool operator!=( const self_type & rhs ) { return ( ( not _same_G( rhs ) ) or ( _i != rhs._i ) or ( _l != rhs._l ) ); }
                bool operator<( const self_type & rhs )  { assert( _same_G( rhs ) ); return ( _l <  rhs._l ); }
                bool operator>( const self_type & rhs )  { assert( _same_G( rhs ) ); return ( _l >  rhs._l ); }
                bool operator<=( const self_type & rhs ) { assert( _same_G( rhs ) ); return ( _l <= rhs._l ); }
                bool operator>=( const self_type & rhs ) { assert( _same_G( rhs ) ); return ( _l >= rhs._l ); }
                operator bool() const { return _l < _G._begin[ _i ] + _G._degree[ _i ]; }
                value_type j_th( const int j ) { assert( j >= 0 ); assert( j < _G._degree[ _i ] ); return _G._nn[ _G._begin[ _i ] + j ]; }
            private:
                const StaticGraph &    _G;
                const int              _i;
                int                    _l;
        };
        */

        t_node_iterator node_it() const { return t_node_iterator( ( * this ) ); }
        //t_node_iterator end()   { return t_node_iterator( _num_nodes ); }        
        //t_const_node_iterator begin() const { return t_const_node_iterator();             }
        //t_const_node_iterator end()   const { return t_const_node_iterator( _num_nodes ); }       

        t_nn_iterator nn_it( const int i ) const { return t_nn_iterator( ( * this ) , i ); }
        //t_nn_iterator nn_end( const int i )   { assert( has_node( i ) ); t_nn_iterator( ( * this ) , i ); }
        //t_const_nn_iterator nn_begin( const int i ) { assert( has_node( i ) ); t_const_nn_iterator( i ); }
        //t_const_nn_iterator nn_end( const int i )   { assert( has_node( i ) ); t_const_nn_iterator( i , _degree[ i ] ); }

        StaticGraph( int num_nodes , const std::set< std::pair< int , int > > & links ) {

            _num_nodes = num_nodes;
            _num_links = links.size(); 

            // Check that the links are well formed and construct the double_links_list
            std::vector< std::pair< int , int > > double_links_list( 2 * _num_links );
            int l = 0;
            for ( std::set< std::pair< int , int > >::iterator l_it = links.begin() ; l_it != links.end() ; l_it++ ) {
                int i = ( * l_it ).first;
                int j = ( * l_it ).second;
                if ( ( i < 0 ) or ( i >= _num_nodes ) ) { std::cout << "ERROR @ StaticGraph.StaticGraph : invalid node number i = " << i << std::endl; throw; }
                if ( ( j < 0 ) or ( j >= _num_nodes ) ) { std::cout << "ERROR @ StaticGraph.StaticGraph : invalid node number j = " << j << std::endl; throw; }
                if ( i == j ) { std::cout << "ERROR @ StaticGraph.StaticGraph : self-edges not allowed." << std::endl; throw; }
                double_links_list[ l ].first = i;
                double_links_list[ l ].second = j;
                l++;
                double_links_list[ l ].first  = j;
                double_links_list[ l ].second = i;
                l++;
            }            
	
            // Sorting the double_links_list
            std::sort( std::begin( double_links_list ) , std::end( double_links_list ) );

            // Initialize _nn, _degree and _begin; the doubl_links_list is used to initialize _nn, that is its purpose.
            _nn.resize( 2 * _num_links );
            _degree.resize( _num_nodes );
            _begin.resize( _num_nodes );

            for ( int i = 0 ; i < _num_nodes ; i++ ) { _degree[ i ] = 0; }

            int i_prev = -1;
            for ( l = 0 ; l < ( int ) double_links_list.size() ; l++ ) {
                int i = double_links_list[ l ].first;
                int j = double_links_list[ l ].second;

                // FOR TESTING.
                //std::cout << "( " << i << " , " << j << " )" << std::endl;

                _degree[ i ]++;
                _nn[ l ] = j;
                if ( i != i_prev ) {
                    i_prev = i;
                    _begin[ i ] = l;
                }
            }
            
        }
        StaticGraph( const StaticGraph & G ) : _num_nodes( G._num_nodes ) , _num_links( G._num_links ) , _nn( G._nn ) , _begin( G._begin ) , _degree( G._degree ) {}
        ~StaticGraph() {}
 
        StaticGraph & operator=( const StaticGraph & G ) {
            _num_nodes = G._num_nodes;
            _num_links = G._num_links;
            _nn        = G._nn;
            _begin     = G._begin;
            _degree    = G._degree;
            return ( * this );
        }

        bool operator==( const StaticGraph & rhs ) const {
            if ( _num_nodes != rhs._num_nodes ) { return false; }
            if ( _num_links != rhs._num_links ) { return false; }
            if ( _nn != rhs._nn )               { return false; }
            if ( _begin != rhs._begin )         { return false; }
            if ( _degree != rhs._degree )       { return false; }
            return true;
        }
        bool operator!=( const StaticGraph & rhs ) { return ( not ( ( * this ) == rhs ) ); }

        int num_nodes() const { return _num_nodes; }
        int num_links() const { return _num_links; }
        int degree( int i ) const { assert( has_node( i ) ); return _degree[ i ]; }
        double avrg_degree() const { return ( ( double ) 2 * _num_nodes ) / ( ( double ) _num_links ); }
       
        bool has_node( int i ) const { return ( ( i >= 0 ) and ( i < _num_nodes ) ); }
        bool has_nn( int i , int j ) const {
            // By bijection method, takes ~ log_2( k ) time steps.
            assert( has_node( i ) );
            int l_left  = _begin[ i ];
            int l_right = _begin[ i ] + _degree[ i ] - 1;
            while ( l_right - l_left > 1 ) {
                int l_pivot = ( l_right + l_left ) / 2;
                if ( j < _nn[ l_pivot ] ) {
                    l_right = l_pivot;
                } else {
                    l_left = l_pivot;
                }
            }
            if ( _nn[ l_left  ] == j ) { return true; }
            if ( _nn[ l_right ] == j ) { return true; }
            return false;
        }
        bool has_link( int i , int j ) const { return has_nn( i , j ); }

        t_nn_iterator find_nn_it( int i , int j ) const {
            // By bijection method, takes ~ log_2( k ) time steps.
            assert( has_node( i ) );
            int l_left  = _begin[ i ];
            int l_right = _begin[ i ] + _degree[ i ] - 1;
            while ( l_right - l_left > 1 ) {
                int l_pivot = ( l_right + l_left ) / 2;
                if ( j < _nn[ l_pivot ] ) {
                    l_right = l_pivot;
                } else {
                    l_left = l_pivot;
                }
            }
            t_nn_iterator _nn_it = nn_it( i );
            if ( _nn[ l_left  ] == j ) {
                _nn_it._l = l_left;
                return _nn_it; 
            }
            if ( _nn[ l_right ] == j ) { 
                _nn_it._l = l_right;
                return _nn_it; 
            }
            _nn_it._l = _begin[ i ] + _degree[ i ]; // Thus ( bool nn_it ) evaluates to false.
            //_nn_it._l = _nn.size(); // Thus ( bool nn_it ) evaluates to false.
            return _nn_it;
        }

    private:

        int                    _num_nodes;
        int                    _num_links;
        std::vector< int >     _nn;
        std::vector< int >     _begin;
        std::vector< int >     _degree;

};

typedef StaticGraph::t_nn_iterator     t_static_graph_nn_iterator;

#endif // Included_cpp_static_graph
