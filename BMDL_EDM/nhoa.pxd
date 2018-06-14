# === Cimports ===

#cimport numpy as np
#cimport cython
#from libcpp.set cimport set as cset
#from libcpp.map cimport map as cmap
from cython.operator cimport dereference as deref, preincrement as inc

#from graph cimport *
from edm cimport *

# === Exposing the C++ NHOA ===

cdef extern from "cpp_nhoa.h":
    double run_nhoa( ExternalDegreeModel & edm , double beta , long int loop_max , double err_tol , bint verbose , bint one_cluster_per_node , bool & failed_convergence )
    double sample_nhoa( ExternalDegreeModel & edm , ExternalDegreeModel & best_edm , double beta , long int loop_max , double err_tol , bool verbose , int repetitions , bool & failed_convergence )
