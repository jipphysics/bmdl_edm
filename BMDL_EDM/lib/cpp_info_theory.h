#ifndef Included_cpp_info_theory
#define Included_cpp_info_theory

#include <cstddef>
//#include <set>
#include <map>
#include <vector>
//#include <array>        // std::array
#include <iostream>
#include <cassert>
//#include <algorithm>    // std::random_shuffle
//#include <ctime>        // std::time
//#include <cstdlib>      // std::rand, std::srand
#include <math.h>         // exp , log , sqrt
#include <cmath>          // std::abs , std::lgamma
//#include <limits>         // std::numeric_limits<double>::infinity()
//#include <chrono>
//#include "cpp_partition.h"

double _xlnx( double x ) {
    assert( x >= 0. );
    assert( x <  1. );
    if ( x > 0. ) { return x * std::log( x ); }
    return 0.;
};

double _nlnn( double n ) {
    assert( n >= 0. );
    if ( n > 0. ) { return n * std::log( n ); }
    return 0.;
};


//double entropy( Partition & part ) {
//    double suma = 0.;
//    double denominator = ( double ) part.total_count();
//    for ( Partition::t_label_iterator l_it = part.label_it() ; l_it ; ++l_it ) {
//        int l = ( * l_it );
//        double p = ( ( double ) part.label_count( l ) ) / denominator;
//        suma -= p * std::log( p );
//    }
//    return suma;
//}

//double joint_entropy( Partition & part1 , Partition part2 ) {
    // The joint entropy is defined and computed as:
    // H( p , p' ) = - sum_{ i in p , i' in p'} P( i i' ) ln P( i i' )
    //             = ln( N ) - ( 1 / N ) sum_{ i in p , i' in p'} N( i i' ) ln N( i i' )
    // where:
    // N = sum_{ i in p , i' in p'} N( i i' )
    // P( i i' ) = N( i i' ) / N
    // Additionally, u1 is the set of nodes corresponding to label l1,
    // and u1Nu2 is the set of nodes that is obtained from the intersection between u1 and u2 where u2 is assumed to be the analogous of u1 but for l2 instead of l1.
//    double N = 0.
//    double suma = 0.
//    for ( Partition::t_label_iterator l1_it = part.label_it() ; l1_it ; ++l1_it ) {
//        int l1 = ( * l1_it );
//        std::set< int > u1;
//        for ( Partition::t_labelset_iterator i1_it = part.labelset_it( l1 ) ; i1_it ; ++i1_it ) {
//            int i1 = ( * i1_it );
//            u1.insert( i1 );  
//        }
//        for ( Partition::t_label_iterator l2_it = part.label_it() ; l2_it ; ++l2_it ) {
//            int l2 = ( * l2_it );
//            std::set< int > std::set< int > u1Nu2;
//            u1Nu2 = u1;
//            for ( LabeltoNodes::t_labelset_iterator i2_it = part.labelset_it( l2 ) ; i2_it ; ++i2_it ) {
//                int i2 = ( * i2_it );
//                u1Nu2.insert( i2 );
//            }
//            double N12 = ( double ) u1Nu2.size(); 
//            N_total += N12;
//            suma += N12 * std::log( N12 );
//        }
//    }
//    assert( N > 0. );
//    return std::log( N ) - suma / N;
//}

template < class T > class Matrix {

    friend std::ostream& operator<<( std::ostream& os , Matrix< T > & m ) {
        std::string sep = " ";
        os << "Matrix[ " << m.num_rows() << " x " << m.num_cols() <<" ]{" << std::endl;
        for ( int j = 0 ; j < m.num_cols() ; j++ ) {
            for ( int i = 0 ; i < m.num_rows() ; i++ ) {
                os << m.get_value( i , j ) << " ";
            }
            os << std::endl;
        }
        os << "}"; // << std::endl;       
        return os;    
    }

    public:

        Matrix< T >() : _num_rows( 0 ) , _num_cols( 0 ) { _data.resize( 0 ); }
        Matrix< T >( const int num_rows , const int num_cols ) : _num_rows( num_rows ) , _num_cols( num_cols ) { resize( _num_rows , _num_cols ); }
        Matrix< T >( const int rank ) : _num_rows( rank ) , _num_cols( rank ) { resize( _num_rows , _num_cols ); }
        Matrix< T >( const Matrix< T > & m ) : _num_rows( m._num_rows ) , _num_cols( m._num_cols ) , _data( m._data ) {}
        ~Matrix() {}

        Matrix< T > & operator=( const Matrix< T > & other ) {
            _num_rows                   = other._G;
            _num_cols                   = other._part;
            _data                       = other._data;
            return ( * this );
        }

        void resize( const int num_rows , const int num_cols ) { 
            assert( num_rows > 0 );
            assert( num_cols > 0 );
            _num_rows = num_rows;
            _num_cols = num_cols;
            _data.resize( _num_rows * _num_cols ); 
        }

        /*
        void soft_resize( const int num_rows , const int num_cols ) { 
            assert( num_rows > 0 );
            assert( num_cols > 0 );
            _num_rows = num_rows;
            _num_cols = num_cols;
            if ( _num_rows * _num_cols > _data.size() ) { _data.resize( _num_rows * _num_cols ); }
        }*/

        void to_const( const T c ) { for ( int t = 0 ; t < ( int ) _data.size() ; t++ ) { _data[ t ] = c; } }
        void to_zeros() { to_const( ( T ) 0 ); }
        void to_ones()  { to_const( ( T ) 1 ); }

        void set_value( const int i , const int j , const T value ) {
            assert( i >= 0 );
            assert( i < _num_rows );
            // FOR TESTING.
            //std::cout << "@ Matrix.set_value : i=" << i << " _num_rows=" << _num_rows << std::endl;
            assert( j >= 0 );
            // FOR TESTING.
            //std::cout << "@ Matrix.set_value : j=" << j << " _num_cols=" << _num_cols << std::endl;
            assert( j < _num_cols );
            _data[ i + j * _num_rows ] = value;
        }

        T get_value( const int i , const int j ) const {
            assert( i >= 0 );
            assert( i < _num_rows );
            assert( j >= 0 );
            assert( j < _num_cols );
            return _data[ i + j * _num_rows ];
        }

        void inc( const int i , const int j ) {
            assert( i >= 0 );
            assert( i < _num_rows );
            // FOR TESTING.
            //std::cout << "@ Matrix.set_value : i=" << i << " _num_rows=" << _num_rows << std::endl;
            assert( j >= 0 );
            // FOR TESTING.
            //std::cout << "@ Matrix.set_value : j=" << j << " _num_cols=" << _num_cols << std::endl;
            assert( j < _num_cols );
            _data[ i + j * _num_rows ]++;
        }

        void dec( const int i , const int j ) {
            assert( i >= 0 );
            assert( i < _num_rows );
            // FOR TESTING.
            //std::cout << "@ Matrix.set_value : i=" << i << " _num_rows=" << _num_rows << std::endl;
            assert( j >= 0 );
            // FOR TESTING.
            //std::cout << "@ Matrix.set_value : j=" << j << " _num_cols=" << _num_cols << std::endl;
            assert( j < _num_cols );
            _data[ i + j * _num_rows ]--;
        }

        T sum_row( const int i ) const {
            assert( i >= 0 ); assert( i < _num_rows );
            T suma = ( T ) 0;
            for ( int j = 0 ; j < _num_cols ; j++ ) { suma += get_value( i , j ); }
            return suma;
        }
        T sum_col( const int j ) const {
            assert( j >= 0 ); assert( j < _num_cols );
            T suma = ( T ) 0;
            for ( int i = 0 ; i < _num_rows ; i++ ) { suma += get_value( i , j ); }
            return suma;
        }
        T sum_total() const {
            T suma = ( T ) 0;
            for ( int i = 0 ; i < _num_rows ; i++ ) { for ( int j = 0 ; j < _num_cols ; j++ ) { suma += get_value( i , j ); } }
            return suma;
        }

        std::vector< T > vector_sum_rows() const {
            std::vector< T > v( _num_rows );
            for ( int i = 0 ; i < _num_rows ; i++ ) {
                v[ i ] = sum_row( i );
            }
            return v;
        }
        std::vector< T > vector_sum_cols() const {
            std::vector< T > v( _num_cols );
            for ( int j = 0 ; j < _num_cols ; j++ ) {
                v[ j ] = sum_col( j );
            }
            return v;
        }

        //Matrix< T > operator*( const Matrix< T > m ) const {
        //    // Matrix times Matrix
        //    Matrix< T > r( _num_rows , m._num_cols );
        //    for ( int i = 0 ; i < _num_rows ; i++ ) {
        //        for ( int j = 0 ; j < m._num_cols ; j++ ) {
        //            T suma = ( T ) 0;
        //            for ( int k = 0 ; k < _num_cols ; k++ ) {
        //                suma += get_value( i , k ) * m.get_value( k , j );
        //            }
        //            r.set_value( i , j , suma );
        //        }
        //    } 
        //    return r;
        //}
        //Matrix< T > operator*( const T c ) const {
        //    // Matrix times scalar
        //    Matrix< T > r( _num_rows , _num_cols );
        //    for ( int i = 0 ; i < _num_rows ; i++ ) {
        //        for ( int j = 0 ; j < m._num_cols ; j++ ) {
        //            r.set_value( i , j , c * m.get_value( i , j ) );
        //        }
        //    } 
        //    return r;
        //}

        int num_rows() const { return _num_rows; }
        int num_cols() const { return _num_cols; }
        int size() const { return _num_rows * _num_cols; }

    private:

        friend double joint_entropy( const Matrix< int > & Nij );

        int                 _num_rows;
        int                 _num_cols;
        std::vector< T >    _data;

};
typedef Matrix< int > t_Matrix_int;

class VectorSet {

    public:

        VectorSet( const int s ) : _data( s ) , _count( 0 ) { reset(); }
        VectorSet( const VectorSet & vs ) : _data( vs._data ) , _count( vs._count ) {}
        ~VectorSet() {}

        VectorSet & operator=( const VectorSet & other ) {
            _data                   = other._data;
            _count                  = other._count;
            return ( * this );
        }

        void reset() { _count = 0; for ( int i = 0 ; i < size() ; i++ ) { _data[ i ] = false; } }
        void resize( const int s ) { _data.resize( s ); reset(); }
        int size() const { return _data.size(); }

        void insert( const int i ) {
            if ( not _data[ i ] ) { _count++; }
            _data[ i ] = true;
        }
        const bool operator[] ( const int i ) const {
            return _data[ i ];
        }
        int count() const { return _count; }

        VectorSet set_union( const VectorSet & vs ) const {
            assert( size() == vs.size() );
            VectorSet uvs( size() );
            for ( int i = 0 ; i < size() ; i++ ) {
                if ( vs[ i ] or ( * this )[ i ] ) { uvs.insert( i ); }
            }
            return uvs;
        }
        VectorSet set_intersection( const VectorSet & vs ) const {
            assert( size() == vs.size() );
            VectorSet ivs( size() );
            for ( int i = 0 ; i < size() ; i++ ) {
                if ( vs[ i ] and ( * this )[ i ] ) { ivs.insert( i ); }
            }
            return ivs;
        }

    private:

        std::vector< bool >    _data;
        int                    _count;

};

void compute_Nij( const Partition & part_i , const Partition & part_j , Matrix< int > & Nij ) {
//void compute_Nij( Partition & part_i , Partition & part_j , Matrix< int > & Nij ) {

    int num_elements = part_i.size();
    assert( part_i.size() == part_j.size() );

    // FOR TESTING.
    //std::cout << "part_i" << std::endl;
    //part_i.cout_self();
    //std::cout << "part_j" << std::endl;
    //part_j.cout_self();

    // Enumerates row labels.
    std::map< int , int > map_li_to_ri;
    int ri = 0;
    for ( Partition::t_label_iterator l_it = part_i.label_it() ; l_it ; ++l_it ) {
        int li = ( * l_it );
        map_li_to_ri[ li ] = ri;
        ri++;
    }

    // Enumerates col labels.
    std::map< int , int > map_lj_to_rj;
    int rj = 0;
    for ( Partition::t_label_iterator l_it = part_j.label_it() ; l_it ; ++l_it ) {
        int lj = ( * l_it );
        map_lj_to_rj[ lj ] = rj;
        rj++;
    }

    //std::cout << "map_lj.size()=" << map_lj.size() << std::endl;

    // Resize matrix Nij.
    if ( ( Nij.num_rows() != ri ) or ( Nij.num_cols() != rj ) ) { Nij.resize( ri , rj ); }

    // Set to zero the Nij entries.
    Nij.to_zeros();    

    // FOR TESTING.
    //std::cout << "@ compute_Nij : Nij = " << Nij << std::endl;
    //std::cout << "Nij.num_rows()=" << Nij.num_rows() << std::endl;
    //std::cout << "Nij.num_cols()=" << Nij.num_cols() << std::endl;

    for ( int e = 0 ; e < num_elements ; e++ ) {
        int li = part_i.label_of_element( e );
        int lj = part_j.label_of_element( e );
        int ri = map_li_to_ri[ li ];
        int rj = map_lj_to_rj[ lj ];

        // FOR TESTING.
        //std::cout << "e=" << e << " li=" << li << " lj=" << lj << " ri=" << ri << " rj=" << rj << std::endl;

        Nij.inc( ri , rj );
    }
    
    // FOR TESTING.
    //std::cout << Nij << std::endl;

}

double entropy( const std::vector< int > & v ) {
    double N = 0.;
    double suma = 0.;
    for ( int t = 0 ; t < ( int ) v.size() ; t++ ) {
        double n = ( double ) v[ t ];
        N += n;
        suma += _nlnn( n );
    }
    //assert( N > 0. );
    if ( N > 0. ) { return std::log( N ) - suma / N; }
    return 0.;
}

double joint_entropy( const Matrix< int > & Nij ) {
    //return entropy( Nij._data );
    double N = 0.;
    double suma = 0.;
    for ( int i = 0 ; i < ( int ) Nij.num_rows() ; i++ ) {
        for ( int j = 0 ; j < ( int ) Nij.num_cols() ; j++ ) {
            double nij = ( double ) Nij.get_value( i , j );
            N += nij;
            suma += _nlnn( nij );
        }
    }
    //assert( N > 0. );
    if ( N > 0. ) { return std::log( N ) - suma / N; }
    return 0.;
}

double MI( Matrix< int > & Nij ) {
    // Mutual Information.
    std::vector< int > Ni = Nij.vector_sum_rows();
    std::vector< int > Nj = Nij.vector_sum_cols();
    double Hi  = entropy( Ni );
    double Hj  = entropy( Nj );
    double Hij = joint_entropy( Nij );
    return Hi + Hj - Hij;
}

double NMIa( Matrix< int > & Nij ) {
    // Normalized Mutual Information, "arithmetic" version.
    std::vector< int > Ni = Nij.vector_sum_rows();
    std::vector< int > Nj = Nij.vector_sum_cols();
    double Hi  = entropy( Ni );
    double Hj  = entropy( Nj );
    double Hij = joint_entropy( Nij );
    double deno = Hi + Hj;
    if ( deno > 0. ) {
        return 2. * ( Hi + Hj - Hij ) / deno;
    }
    //return -1.;
    return 0.;
}
double NMIg( Matrix< int > & Nij ) {
    // Normalized Mutual Information, "geometric" version.
    std::vector< int > Ni = Nij.vector_sum_rows();
    std::vector< int > Nj = Nij.vector_sum_cols();
    double Hi  = entropy( Ni );
    double Hj  = entropy( Nj );
    double Hij = joint_entropy( Nij );
    double deno = std::sqrt( Hi * Hj );
    if ( deno > 0. ) {
        return ( Hi + Hj - Hij ) / deno;
    }
    //return -1.;
    return 0.;
}
double NMIm( Matrix< int > & Nij ) {
    // Normalized Mutual Information, "max" version.
    std::vector< int > Ni = Nij.vector_sum_rows();
    std::vector< int > Nj = Nij.vector_sum_cols();
    double Hi  = entropy( Ni );
    double Hj  = entropy( Nj );
    double Hij = joint_entropy( Nij );
    double deno = std::max( Hi , Hj );
    if ( deno > 0. ) {
        return ( Hi + Hj - Hij ) / deno;
    }
    //return -1.;
    return 0.;
}
double NMImi( Matrix< int > & Nij ) {
    // Normalized Mutual Information, "max" version.
    std::vector< int > Ni = Nij.vector_sum_rows();
    std::vector< int > Nj = Nij.vector_sum_cols();
    double Hi  = entropy( Ni );
    double Hj  = entropy( Nj );
    double Hij = joint_entropy( Nij );
    double deno = std::min( Hi , Hj );
    if ( deno > 0. ) {
        return ( Hi + Hj - Hij ) / deno;
    }
    //return -1.;
    return 0.;
}
// By default, the NMI is the NMIm.
double NMI( Matrix< int > & Nij ) { return NMIm( Nij ); }

double EMI( const Matrix< int > & Nij ) {
    // Expected Mutual Information.
    // Refs.:
    // [1] https://en.wikipedia.org/wiki/Adjusted_mutual_information
    // [2] Vinh, Nguyen Xuan; Epps, Julien; Bailey, James (2010), "Information Theoretic Measures for Clusterings Comparison: Variants, Properties, Normalization and Correction for Chance" (PDF), The Journal of Machine Learning Research, 11 (oct): 2837–54
    std::vector< int > Ni = Nij.vector_sum_rows();
    std::vector< int > Nj = Nij.vector_sum_cols();
    int N = Nij.sum_total();
    assert( N > 0 );
    double d_N = ( double ) N;
    double suma = 0.;
    for ( int i = 0 ; i < Nij.num_rows() ; i++ ) {
        for( int j = 0 ; j < Nij.num_cols() ; j++ ) {
            int ai = Ni[ i ];
            int bj = Nj[ j ];
            assert( ai > 0 );
            assert( bj > 0 );
            double d_ai = ( double ) ai;
            double d_bj = ( double ) bj;
            for ( int nij = std::max( 1 , ai + bj - N ) ; nij <= std::min( ai , bj ) ; nij++ ) {
                double d_nij = ( double ) nij;
                double factor1 = std::log( d_N ) + std::log( d_nij ) - std::log( d_ai ) - std::log( d_bj );
                factor1 *= d_nij / d_N;
                double factor2 = std::lgamma( d_ai + 1 ) + std::lgamma( d_bj + 1 );
                factor2 += std::lgamma( d_N - d_ai + 1 ) + std::lgamma( d_N - d_bj + 1 );
                factor2 -= std::lgamma( d_N + 1 );
                factor2 -= std::lgamma( d_nij + 1 );
                factor2 -= std::lgamma( d_ai - d_nij + 1 ) + std::lgamma( d_bj - d_nij + 1 );
                factor2 -= std::lgamma( d_N - d_ai - d_bj + d_nij + 1 );
                factor2 = std::exp( factor2 );
                suma += factor1 * factor2;
            }
        }
    }
    return suma;
}

double AMI( const Matrix< int > & Nij ) {
    // Adjusted Mutual Information.
    // Refs.:
    // [1] https://en.wikipedia.org/wiki/Adjusted_mutual_information
    // [2] Vinh, Nguyen Xuan; Epps, Julien; Bailey, James (2010), "Information Theoretic Measures for Clusterings Comparison: Variants, Properties, Normalization and Correction for Chance" (PDF), The Journal of Machine Learning Research, 11 (oct): 2837–54
    std::vector< int > Ni = Nij.vector_sum_rows();
    std::vector< int > Nj = Nij.vector_sum_cols();
    double Hi  = entropy( Ni );
    double Hj  = entropy( Nj );
    double Hij = joint_entropy( Nij );
    double _MI = Hi + Hj - Hij;
    double _EMI = EMI( Nij );
    double deno = std::max( Hi , Hj ) - _EMI;
    if ( deno != 0. ) {
        return ( _MI - EMI( Nij ) ) / deno;
    }
    //return std::numeric_limits<double>::quiet_NaN();
    return -1.;
}

void compute_all_info_measures( const Matrix< int > & Nij , double & __Hi , double & __Hj , double & __Hij , double & __MI , double & __NMIa , double & __NMIg , double & __NMIm , double & __NMImi , double & __EMI , double & __AMI ) {

    std::vector< int > Ni = Nij.vector_sum_rows();
    std::vector< int > Nj = Nij.vector_sum_cols();

    // We do not directly use the variables __Hi, __Hj, etc. because if the same dummy variable is passed as arguments of more than one of those variables __Hi, __Hj, etc., then an incorrect result may occur.
    double _Hi  = entropy( Ni );
    double _Hj  = entropy( Nj );
    double _Hij = joint_entropy( Nij );
    double _MI  = _Hi + _Hj - _Hij;

    double max_H = std::max( _Hi , _Hj );
    double min_H = std::min( _Hi , _Hj );
    double deno;

    double _NMIa;
    deno = _Hi + _Hj;
    //if ( max_H > 0. ) {
    if ( deno > 0. ) {
        _NMIa = 2. * _MI / deno;
    } else {
        //_NMIa = -1.; // Returns an impossible to report that the NMI couldn't be computed.
        _NMIa = 0.;
    }

    double _NMIg;
    deno = _Hi * _Hj;
    if ( deno > 0. ) {
        _NMIg = _MI / std::sqrt( deno );
    } else {
        //_NMIg = -1.; // Returns an impossible to report that the NMI couldn't be computed.
        _NMIg = 0.;
    }

    double _NMIm;
    if ( max_H > 0. ) {
        _NMIm = _MI / max_H;
    } else {
        //_NMIm = -1.; // Returns an impossible to report that the NMI couldn't be computed.
        _NMIm = 0.;
    }

    double _NMImi;
    if ( min_H > 0. ) {
        _NMImi = _MI / min_H;
    } else {
        //_NMImi = -1.; // Returns an impossible to report that the NMI couldn't be computed.
        _NMImi = 0.;
    }

    double _EMI = EMI( Nij );

    double _AMI; //= -1.; // Returns an impossible to report that the AMI couldn't be computed.
    deno = max_H - _EMI;
    //if ( ( _EMI >= 0. ) and ( deno != 0. ) ) { 
    if ( deno > 0. ) { 
        _AMI = ( _MI - _EMI ) / deno;
    } else {
        _AMI = 0.;
    }

    __Hi    = _Hi;
    __Hj    = _Hj;
    __Hij   = _Hij;
    __MI    = _MI;
    __NMIa  = _NMIa;
    __NMIg  = _NMIg;
    __NMIm  = _NMIm;
    __NMImi = _NMImi;
    __EMI   = _EMI;
    __AMI   = _AMI;

}

#endif // Included_cpp_info_theory
