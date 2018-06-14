#include <cstddef>
//#include <set>
//#include <map>
//#include <vector>
//#include <array>        // std::array
#include <iostream>
#include <cassert>
//#include <algorithm>    // std::random_shuffle
//#include <ctime>        // std::time
//#include <cstdlib>      // std::rand, std::srand
//#include <math.h>       // exp , log , sqrt
//#include <cmath>        // std::abs , std::lgamma
//#include <limits>       // std::numeric_limits<double>::infinity()
#include "cpp_partition.h"
//#include "cpp_utils.h"
#include "cpp_info_theory.h"

// === For testing purposes ===.

int main() {

    Partition part1( 7 );
    Partition part2( 7 );

    std::cout << "part1 = " << part1 << std::endl;
    std::cout << "part2 = " << part2 << std::endl;
    std::cout << std::endl;

    // [ 0 2 4 ] [ 1 5 ] [ 3 6 ]
    part1.change_label_of_element( 0 , 0 );
    part1.change_label_of_element( 2 , 0 );
    part1.change_label_of_element( 4 , 0 );
    part1.change_label_of_element( 5 , 1 );
    part1.change_label_of_element( 3 , 2 );
    part1.change_label_of_element( 6 , 2 );

    // [ 1 3 6 ] [ 4 5 ] [ 0 2 3 ]
    part2.change_label_of_element( 1 , 0 );
    part2.change_label_of_element( 3 , 0 );
    part2.change_label_of_element( 6 , 0 );
    part2.change_label_of_element( 4 , 1 );
    part2.change_label_of_element( 5 , 1 );
    part2.change_label_of_element( 2 , 2 );
    part2.change_label_of_element( 3 , 2 );
    part2.change_label_of_element( 4 , 2 );

    std::cout << "part1 = " << part1 << std::endl;
    std::cout << "part2 = " << part2 << std::endl;
    std::cout << std::endl;

    Matrix< int > Nij;

    std::cout << "Nij just created..." << std::endl;   
    std::cout << std::endl;

    compute_Nij( part1 , part2 , Nij );    

    std::cout << "Nij just computed..." << std::endl;   
    std::cout << "Nij=" << Nij << std::endl;   
    std::cout << std::endl;

    std::cout << "MI=" << MI( Nij ) << std::endl;
    std::cout << "NMI=" << NMI( Nij ) << std::endl;
    std::cout << "EMI=" << EMI( Nij ) << std::endl;
    std::cout << "AMI=" << AMI( Nij ) << std::endl;
    std::cout << std::endl;

    double Hi;
    double Hj;
    double Hij;
    double MI;
    double NMIa;
    double NMIg;
    double NMIm;
    double NMImi;
    double EMI;
    double AMI;
    compute_all_info_measures( Nij , Hi , Hj , Hij , MI , NMIa , NMIg , NMIm , NMImi , EMI , AMI );

    std::cout << "Compute all ..." << std::endl;
    std::cout << "Hi=" << Hi << std::endl;
    std::cout << "Hj=" << Hj << std::endl;
    std::cout << "Hij=" << Hij << std::endl;
    std::cout << "MI=" << MI << std::endl;
    std::cout << "NMIa=" << NMIa << std::endl;
    std::cout << "NMIg=" << NMIg << std::endl;
    std::cout << "NMIm=" << NMIm << std::endl;
    std::cout << "NMImi=" << NMImi << std::endl;
    std::cout << "EMI=" << EMI << std::endl;
    std::cout << "AMI=" << AMI << std::endl;
    std::cout << std::endl;

}
