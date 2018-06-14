# distutils: language = c++
# distutils: sources = ExternalDegreeModel/lib/cpp_graph.cpp ExternalDegreeModel/lib/cpp_edm.cpp ExternalDegreeModel/lib/cpp_nhoa.cpp

# === Cimports ===

#cimport numpy as np
#cimport cython
#from libcpp.set cimport set as cset
#from libcpp.map cimport map as cmap
from cython.operator cimport dereference as deref, preincrement as inc

# === Python wrapper for the C++ NHOA functions ===

def Py_re_seed_eng():
    #print "Initializing the C++ engine for pseudo-random number generation."
    init_random_eng()
