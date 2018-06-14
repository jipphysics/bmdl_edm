#ifndef Included_cpp_utils
#define Included_cpp_utils

#include <cstddef>
//#include <set>
#include <map>
#include <vector>
//#include <array>        // std::array
#include <iostream>
#include <cassert>
#include <math.h>         // exp , log , sqrt
#include <cmath>          // std::abs , std::lgamma
#include <algorithm>      // std::random_shuffle
#include <ctime>          // std::time
#include <cstdlib>        // std::rand, std::srand
#include <chrono>

// *** Useful typedef ***

typedef std::vector< double >                                                 tv_1;
typedef std::vector< std::vector< double > >                                  tv_2;
typedef std::vector< std::vector< std::vector< double > > >                   tv_3;
typedef std::vector< std::vector< std::vector< std::vector< double > > > >    tv_4;

// *** swap ***

//template <typename T>
//void swap( T a , T b ) {
//    T x = a;
//    a = b;
//    b = x;
//}

// *** rel_diff ***

double rel_diff( double x , double y ) {
    double diff = x - y;
    if ( diff == 0. ) { 
        return 0.; 
    } else {
        return ( diff + diff ) / ( abs( x ) + abs( y ) );
    }
}

// *** xlnx ***

double xlnx( double x ) {
    assert( x >= 0. );
    assert( x <= 1. );
    if ( x == 0. or x == 1. ) return 0.;
    return x * std::log( x );
}

// *** Chrono ***

using time_type       = std::chrono::steady_clock::time_point;
using time_delta_type = std::chrono::duration<double>;
using ns_units        = std::chrono::nanoseconds;
using sec_units       = std::chrono::seconds;
using min_units       = std::chrono::minutes;
using hr_units        = std::chrono::hours;

class Chrono {
    public:
        Chrono() { reset(); }
        ~Chrono() {}
        void reset()  { _t_ini = std::chrono::steady_clock::now(); }
        time_delta_type elapsed() { return std::chrono::steady_clock::now() - _t_ini; }
        double to_ns( time_delta_type td )  { return std::chrono::duration_cast< ns_units >( td ).count(); }
        double to_sec( time_delta_type td ) { return std::chrono::duration_cast< sec_units >( td ).count(); }
        double to_min( time_delta_type td ) { return std::chrono::duration_cast< min_units >( td ).count(); }
        double to_hr( time_delta_type td )  { return std::chrono::duration_cast< hr_units >( td ).count(); }
    private:
        time_type _t_ini;
};

// *** Pure C++ Enumerate class ***

template < class T >
class CPP_Enumerate {
 private:
    int _count;
    std::map< T , int > _enum;
 public:
    CPP_Enumerate() : _count( 0 ) {
        _enum.clear();
    }
    ~CPP_Enumerate() {}
    int insert( T e ) {
         typename std::map< T , int >::iterator e_it = _enum.find( e );
         if ( e_it == _enum.end() ) {
             _enum[ e ] = _count;
             return _count++;
         } else {
             return e_it->second;
         }

    }
    int get( T e ) {
         typename std::map< T , int >::iterator e_it = _enum.find( e );
         if ( e_it == _enum.end() ) return -1;
         return e_it->second;
    }
    int size() { return _count; }
};

#endif // Included_cpp_utils
