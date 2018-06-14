// References:
// [1] https://www.johndcook.com/blog/standard_deviation/

#ifndef Included_cpp_runningstats
#define Included_cpp_runningstats

#include <cassert>
#include <cstddef>
#include <vector>
//#include <set>
//#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>    // std::random_shuffle , std::shuffle (uses a random num. generator, e.g. std::mt19937) , std::max
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <random>       // std::mt19937
#include <functional>   // std::ref
#include <math.h>       // exp , log , sqrt
#include <cmath>        // std::abs
#include <limits>       // std::numeric_limits< double >::infinity() , std::numeric_limits<double>::quiet_NaN()
#include <utility>      // std::pair , std::make_pair
//#include "cpp_bipartite.h"
#include "to_string_patch.h"

class RunningStats {

    friend std::ostream& operator<<( std::ostream& os , RunningStats & rs ) {
       //std::string sep = " ";
       long int _num_samples = rs.num_samples();

       //double _mean          = rs.mean().second;
       //double _variance      = rs.variance().second;
       //double _std           = rs.std().second;
       //double _std_of_mean   = rs.std_of_mean().second;

       double _mean          = rs.mean();
       double _variance      = rs.variance();
       double _std           = rs.std();
       double _std_of_mean   = rs.std_of_mean();

       os << "RunningStats{ mean = " << _mean;
       os << " , variance = " << _variance;
       os << " , std = " << _std;
       os << " , std_of_mean = " << _std_of_mean;
       os << " , num_samples = " << _num_samples;
       os << " }";
       return os;

    } 

    public:
        //typedef sparmat_int_dyn_rowmaj_type::InnerIterator 

        RunningStats() { reset(); }
        RunningStats( const RunningStats & rs ) : _k( rs._k ) , _M_k( rs._M_k ) , _S_k( rs._S_k ) , _min( rs._min ) , _max( rs._max ) {}
        
        ~RunningStats() {}

        RunningStats & operator=( const RunningStats & other ) {
	        _k   = other._k;
            _M_k = other._M_k;
            _S_k = other._S_k;
            _max = other._max;
            _min = other._min;
            return ( * this );
        } 

        void reset() { 
            _k   = 0L; 
            _M_k = 0.; //std::numeric_limits<double>::quiet_NaN();
            _S_k = 0.; //std::numeric_limits<double>::quiet_NaN();
            _min = 0.; //std::numeric_limits<double>::quiet_NaN();
            _max = 0.; //std::numeric_limits<double>::quiet_NaN();
        }

        void insert( double x ) {
            _k++;
            if ( _k == 1 ) {
                _M_k = x;
                _S_k = 0.;
                _min = x;
                _max = x;
            } else {
                double M_old = _M_k;
                double S_old = _S_k;
                _M_k = M_old + ( x - M_old ) / ( ( double ) _k );
                _S_k = S_old + ( x - M_old ) * ( x - _M_k );
                _min = std::min( _min , x );
                _max = std::max( _max , x );
            }
            //std::cout << "_k = "   << patch::to_string( _k ) << " ";
            //std::cout << "_M_k = " << patch::to_string( _M_k ) << " ";
            //std::cout << "_S_k = " << patch::to_string( _S_k ) << " ";
            //std::cout << std::endl;
        }

        long int num_samples() { return _k;            }
        //double   num_samples() { return ( double ) _k; }

        double mean() {
            //std::cout << "mean : _k = " << patch::to_string( _k ) << std::endl;
            //if ( _k < 1L ) { std::cout << "mean : NaN" << std::endl; return std::make_pair( true , 0. ); }
            //return std::make_pair( false , _M_k );
            if ( _k < 1L ) { return std::numeric_limits<double>::quiet_NaN(); }
            return _M_k;
        }
        double variance() {
            //std::cout << "variance : _k = " << patch::to_string( _k ) << std::endl;
            //if ( _k < 2L ) { std::cout << "variance : NaN" << std::endl; return std::make_pair( true , 0. ); }
            //return std::make_pair( false , _S_k / ( ( double ) ( _k - 1 ) ) ); 
            if ( _k < 2L ) { return std::numeric_limits<double>::quiet_NaN(); }
            return _S_k / ( ( double ) ( _k - 1 ) );
        }
        double std() {
            //std::cout << "std : _k = " << patch::to_string( _k ) << std::endl;
            //if ( _k < 2L ) { std::cout << "std : NaN" << std::endl; return std::make_pair( true , 0. ); }
            //return std::make_pair( false , std::sqrt( variance().second ) );
            if ( _k < 2L ) { return std::numeric_limits<double>::quiet_NaN(); }
            return std::sqrt( variance() );
        }
        double std_of_mean() { // Also known as Standard Error of the Mean (SEM).
            //if ( _k < 2L ) { return std::make_pair( true , 0. ); }
            //return std::make_pair( false , std::sqrt( variance().second / ( ( double ) _k ) ) );
            if ( _k < 2L ) { return std::numeric_limits<double>::quiet_NaN(); }
            return std::sqrt( variance() / ( ( double ) _k ) );
        }
        double sem() { return std_of_mean(); }
        double rsem() { return sem() / ( std::abs( mean() ) + 0.01 ); }
        double min() { 
            //if ( _k < 1L ) { return std::make_pair( true , 0. ); }
            //return std::make_pair( false , _min ); 
            if ( _k < 1L ) { return std::numeric_limits<double>::quiet_NaN(); }
            return _min;
        }
        double max() { 
            //if ( _k < 1L ) { return std::make_pair( true , 0. ); }
            //return std::make_pair( false , _max ); 
            if ( _k < 1L ) { return std::numeric_limits<double>::quiet_NaN(); }
            return _max;
        }
        double second_moment() {
            if ( _k < 1L ) { return std::numeric_limits<double>::quiet_NaN(); }
            if ( _k < 2L ) { return _M_k * _M_k; }
            //The variance = <x^2> - <x>^2, so <x^2> = variance + <x>^2. For the calculation, we use the variance without correction; i.e. we divide S_k by k and not by (k-1).
            return _S_k / ( ( double ) _k ) + mean() * mean();
        }

    private:
        long int    _k;   // k = num_samples.
        double      _M_k;
        double      _S_k;
        double      _min;
        double      _max;
};	

#endif // Included_cpp_runningstats
