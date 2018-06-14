#ifndef Included_cpp_label_pool
#define Included_cpp_label_pool

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
//#include <limits>       // std::numeric_limits<double>::infinity()
//#include "cpp_static_graph.h"
#include "to_string_patch.h"

class LabelPool {

    friend std::ostream& operator<<( std::ostream& os , LabelPool & lp );
    public:
        //LabelPool( std::mt19937 & eng ) : _eng( eng ) {}
        LabelPool() { _num_active_labels = 0; }
        LabelPool( int size ) { resize( size ); }
        void resize( int num_labels ) {
            assert( num_labels > 0 );
            _labels.resize( num_labels );
            _labels_idxs.resize( num_labels );
            for ( int i = 0 ; i < num_labels ; i++ ) { 
                _labels[ i ] = i;
                _labels_idxs[ i ] = i;
            }
            _num_active_labels = 0;
        }
        LabelPool( const LabelPool & lp ) : _labels( lp._labels ) , _labels_idxs( lp._labels_idxs ) , _num_active_labels( lp._num_active_labels ) {}
        ~LabelPool() {}

        LabelPool & operator=( const LabelPool & other ) {
            _labels            = other._labels;
            _labels_idxs       = other._labels_idxs;
            _num_active_labels = other._num_active_labels;
            return ( * this );
        }

        int size() const { return _labels.size(); }
        int num_active_labels() const { return _num_active_labels; }
        int num_inactive_labels() const { return size() - _num_active_labels; }
        void assert_label_is_valid( int l ) const {
            assert( ( l >= 0 ) and ( l < size() ) );
        }
        bool is_label_active( int l ) const {
            assert_label_is_valid( l );
            return _labels_idxs[ l ] < num_active_labels();
        }
        int activate_some_label() {
            assert( num_active_labels() < size() );
            return _labels[ _num_active_labels++ ];
        }
        void activate_label( int l ) {
            if ( is_label_active( l ) ) { return; }
            _swap_label( l , _labels[ num_active_labels() ] );
            _num_active_labels++;
        }
        void deactivate_label( int l ) {
            // An active label is deactivated by swapping it with the last active one and then decreasing the number of active labels.
            assert_label_is_valid( l );
            assert( is_label_active( l ) );
            _swap_label( l , _labels[ num_active_labels() - 1 ] );
            _num_active_labels--;
        }
        void _swap_label( int l1 , int l2 ) {
            assert_label_is_valid( l1 );
            assert_label_is_valid( l2 );
            int i1 = _labels_idxs[ l1 ];
            int i2 = _labels_idxs[ l2 ];
            int tmp = _labels[ i1 ];
            _labels[ i1 ] = _labels[ i2 ];
            _labels[ i2 ] = tmp;
            _labels_idxs[ l1 ] = i2;
            _labels_idxs[ l2 ] = i1;
        }
        int operator[] ( const int i ) const {
            // Only active labels can be called by index.
            assert( ( i >= 0 ) and ( i < num_active_labels() ) );
            return _labels[ i ];
        }
        int get_qth_inactive_label( const int q ) const {
            int i = q + _num_active_labels;
            assert( i >= 0 );
            assert( i < size() );
            return _labels[ i ];
        }
        /*
        int random_label( int flag_new ) {
            // flag_new <int> : 0 , 1
            // With uniform probability distribution, it returns one label from the set
            //      { ...active labels... }
            // if flag_new = 0, or from the set
            //      { ...active labels... } U { new active label }
            // if flag_new = 1.
            assert( ( flag_new == 0 ) or ( flag_new == 1 ) );
            int i_max = std::min( num_active_labels() , size() - 1 );
            std::uniform_int_distribution<> dist_i_first( 0 , i_max - 1 + flag_new );
            int i = dist_i_first( _eng );
            int l = _labels[ i ];
            if ( not is_label_active( l ) ) {
                return activate_label();
            }
            return l;
        }*/

    private:
        //std::mt19937 &        _eng;
        std::vector< int >    _labels;
        std::vector< int >    _labels_idxs;
        int                   _num_active_labels;

}; // class LabelPool

class WeightedLabelPool {

    friend std::ostream& operator<<( std::ostream& os , WeightedLabelPool & wlp );
    public:

        WeightedLabelPool ( int size ) { 
            _label_pool.resize( size ); 
            _label_count.resize( size );
            _total_count = 0;
        }
        WeightedLabelPool( const WeightedLabelPool & wlp ) : _label_pool( wlp._label_pool ) , _label_count( wlp._label_count ) , _total_count( wlp._total_count ) {}
        ~WeightedLabelPool() {}

        WeightedLabelPool & operator=( const WeightedLabelPool & other ) {
            _label_pool  = other._label_pool;
            _label_count = other._label_count;
            _total_count = other._total_count;
            return ( * this );
        }

        int size() { return _label_pool.size(); }

        int num_active_labels() const { return _label_pool.num_active_labels(); }
        int num_inactive_labels() const { return _label_pool.num_inactive_labels(); }

        bool is_label_active( int l ) const { return _label_pool.is_label_active( l ); }
        int label_count( int l ) const { return _label_count[ l ]; }
        int total_count() const { return _total_count; }

        /*int activate_some_label() { 
            int l = _label_pool.activate_some_label();
            assert( _label_count[ l ] == 0 );
            _label_count[ l ]++;
            _total_count++;
            return l;
        }*/
        void deactivate_label( int l ) { 
            _label_pool.deactivate_label( l ); 
            _total_count -= _label_count[ l ];
            _label_count[ l ] = 0;
        }
        int inc_label( int l ) {
            // With this function you can activate any chosen label.
            _label_pool.activate_label( l );
            _label_count[ l ]++;
            _total_count++;
            return _label_count[ l ];
        }
        int dec_label( int l ) {
            if ( is_label_active( l ) ) {
                assert( _label_count[ l ] > 0 );
                _label_count[ l ]--;
                _total_count--;
                if ( _label_count[ l ] == 0 ) { _label_pool.deactivate_label( l ); return 0; }
                return _label_count[ l ];
            }
            assert( _label_count[ l ] == 0 );
            return 0;
        }

        const int operator[] ( const int i ) const {
            // Only active labels can be called by index.
            assert( ( i >= 0 ) and ( i < num_active_labels() ) );
            return _label_pool[ i ];
        }
        int get_qth_inactive_label( const int q ) { return _label_pool.get_qth_inactive_label( q ); }

    private:

        LabelPool             _label_pool;
        std::vector< int >    _label_count;
        int                   _total_count;

}; // class WeightedLabelPool

std::ostream& operator<<( std::ostream& os , LabelPool & lp );
std::ostream& operator<<( std::ostream& os , WeightedLabelPool & wlp );

#endif // Included_cpp_label_pool
