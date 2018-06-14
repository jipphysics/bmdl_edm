#ifndef Included_cpp_random
#define Included_cpp_random

#include <cstddef>
//#include <set>
//#include <map>
//#include <vector>
//#include <array>        // std::array
//#include <iostream>
//#include <cassert>
//#include <math.h>         // exp , log , sqrt
//#include <cmath>          // std::abs , std::lgamma
#include <algorithm>      // std::random_shuffle
#include <ctime>          // std::time
#include <cstdlib>        // std::rand, std::srand
#include <functional>   // std::ref
#include <iostream>
#include <fstream>
#include <random>
//#include <chrono>


// *** Engine for the random number generator ***

//
// HOWTO "access" global variables.
// http://www.learncpp.com/cpp-tutorial/42-global-variables/
//
// HOWTO "use" headers.
// https://stackoverflow.com/questions/333889/why-have-header-files-and-cpp-files
//

extern std::mt19937 _eng;
extern void re_seed_rng();

void init_random_eng() { 
    //std::cout << "Initializing the C++ engine for pseudo-random number generation (C++ commenting)." << std::endl;
    re_seed_rng(); 
}

#endif // Included_cpp_random
