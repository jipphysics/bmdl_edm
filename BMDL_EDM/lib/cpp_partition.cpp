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
//#include <cstdlib>      // std::rand, std::srand
#include "cpp_partition.h"  
#include "to_string_patch.h"

// === For testing purposes ===.

int main() {

    Partition part( 5 );
    std::cout << part << std::endl << std::endl;

    std::cout << "Seting element 3 to label 1..." << std::endl;
    part.change_label_of_element( 3 , 1 );
    std::cout << part << std::endl << std::endl;

    std::cout << "Seting element 3 to label 2..." << std::endl;
    part.change_label_of_element( 3 , 2 );
    std::cout << part << std::endl << std::endl;

    std::cout << "Seting element 1 to label 2..." << std::endl;
    part.change_label_of_element( 1 , 2 );
    std::cout << part << std::endl << std::endl;

    std::cout << "Testing copy constructor..." << std::endl;
    Partition part_copy( part );
    std::cout << part_copy << std::endl << std::endl;

    std::cout << "Testing assignement operator..." << std::endl;
    std::cout << part << std::endl;
    part.change_label_of_element( 4 , 0 );
    std::cout << part << std::endl;
    part_copy = part;
    std::cout << part_copy << std::endl << std::endl;

    std::cout << "Testing label iterator..." << std::endl;
    for ( Partition::t_label_iterator l_it = part.label_it() ; l_it ; ++l_it ) {
        int l = ( * l_it );
        std::cout << l << " ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "Testing element iterator..." << std::endl;
    std::cout << "Seting element 4 to label 4..." << std::endl;
    part.change_label_of_element( 4 , 4 );
    std::cout << part << std::endl;
    std::cout << "Iterating..." << std::endl;
    for ( Partition::t_label_iterator l_it = part.label_it() ; l_it ; ++l_it ) {
        int l = ( * l_it );
        std::cout << l << " :" ;
        for ( Partition::t_element_iterator i_it = part.element_it( l ) ; i_it ; ++i_it ) {
            int i ( * i_it );
            std::cout << " " << i;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "Testing _cout_double_linked_lists()..." << std::endl;  
    part._cout_double_linked_lists();

    std::cout << "Testing cout_self()..." << std::endl;  
    part.cout_self();

}

