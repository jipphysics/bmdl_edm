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
#include "to_string_patch.h"
#include "cpp_label_pool.h"
    
std::ostream& operator<<( std::ostream& os , LabelPool & lp ) {
       std::string sep = " ";
       os << "LabelPool{";
       for ( int q = 0 ; q < lp.num_active_labels() ; q++ ) {
           int l = lp[ q ];
           os << sep << patch::to_string( l );
       }
       std::cout << " |";
       for ( int q = 0 ; q < lp.num_inactive_labels() ; q++ ) {
           int l = lp.get_qth_inactive_label( q );
           os << sep << patch::to_string( l );
       }
       os << " }"; // << std::endl;       
       return os;
} 
 
std::ostream& operator<<( std::ostream& os , WeightedLabelPool & wlp ) {
       std::string sep = " ";
       os << "WeightedLabelPool{";
       for ( int q = 0 ; q < wlp.num_active_labels() ; q++ ) {
           int l = wlp[ q ];
           int w = wlp.label_count( l );
           os << sep << patch::to_string( l ) << ":" << patch::to_string( w );
       }
       std::cout << " |";
       for ( int q = 0 ; q < wlp.num_inactive_labels() ; q++ ) {
           int l = wlp.get_qth_inactive_label( q );
           os << sep << patch::to_string( l );
       }
       os << " }"; // << std::endl;       
       return os;
} 

// === For testing purposes ===.

int main() {

    std::cout << "TESTING LabelPool..." << std::endl;

    LabelPool lp( 5 );

    std::cout << lp << std::endl;

    lp.activate_label( 3 );
    lp.activate_label( 1 );

    std::cout << lp << std::endl;    

    int l = lp.activate_some_label();

    std::cout << l << " " << lp << std::endl;    

    lp.deactivate_label( 1 );

    std::cout << lp << std::endl;    

    std::cout << "\nTESTING WeightedLabelPool..." << std::endl;

    WeightedLabelPool wlp( 5 );

    std::cout << wlp << std::endl;

    wlp.inc_label( 3 );
    wlp.inc_label( 1 );

    std::cout << wlp << std::endl;    

    wlp.inc_label( 3 );

    std::cout << wlp << std::endl;    

    wlp.deactivate_label( 3 );

    std::cout << wlp << std::endl; 

    wlp.dec_label( 1 );

    std::cout << wlp << std::endl; 

    std::cout << "\nTESTING WeightedLabelPool : copy constructor..." << std::endl;

    WeightedLabelPool wlp_copy( wlp );

    std::cout << wlp << std::endl;

    std::cout << "\nTESTING WeightedLabelPool : now assignement operator..." << std::endl;

    wlp.inc_label( 4 );
    wlp.inc_label( 1 );
    wlp.inc_label( 4 );

    std::cout << wlp << std::endl;
    std::cout << wlp_copy << std::endl;

    wlp_copy = wlp;

    std::cout << wlp_copy << std::endl;

}
