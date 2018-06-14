# === Cimports ===

#cimport numpy as np
#cimport cython
from libcpp cimport bool
from libcpp.vector cimport vector as cvector
from libcpp.set cimport set as cset
from libcpp.map cimport map as cmap
from libcpp.pair cimport pair as cpair
from cython.operator cimport dereference as deref, preincrement as inc

# === Expossing the cython code in other modules ===

#from graph cimport *

# === Expossing the C++ classes ===

cdef extern from "cpp_partition.h":
    cdef cppclass Partition:
        Partition( int size ) except +
        #~Partition()
        void cout_self()

cdef extern from "cpp_info_theory.h":
    cdef cppclass t_Matrix_int:
        t_Matrix_int() except +
        #~t_Matrix_int()
        void resize( const int num_rows , const int num_cols )
        int num_rows()
        int num_cols()
        int get_value( const int i , const int j )

cdef extern from "cpp_info_theory.h":
    #void compute_Nij( const Partition & part_i , const Partition & part_j , t_Matrix_int & Nij )
    void compute_Nij( Partition & part_i , Partition & part_j , t_Matrix_int & Nij )
    void compute_all_info_measures( t_Matrix_int & Nij , double & __Hi , double & __Hj , double & __Hij , double & __MI , double & __NMIa , double & __NMIg , double & __NMIm , double & __NMImi , double & __EMI , double & __AMI )
    double entropy( cvector[ int ] & v )
    double joint_entropy( t_Matrix_int & Nij )
    double MI( t_Matrix_int & Nij )
    double NMIa( t_Matrix_int & Nij )
    double NMIg( t_Matrix_int & Nij )
    double NMIm( t_Matrix_int & Nij )
    double NMImi( t_Matrix_int & Nij )
    double NMI( t_Matrix_int & Nij )
    double EMI( t_Matrix_int & Nij )
    double AMI( t_Matrix_int & Nij )
    
# === Python Wrapper for the C++ ExternalDegreeModel class ===

#cdef class PyExternalDegreeModel:
#
#    # === Data-members ===
#
#    cdef object                  _G
#    cdef object                  _node_enum
#    cdef object                  _label_enum
#    cdef StaticGraph *           _c_static_graph
#    cdef ExternalDegreeModel *   _c_edm    

    # === Function-members ===

    # @ edm.pyx

# === Complements ===
  
# --- Plot an External Degree Model ---
  
# @ edm.pyx

# --- Compute information theoretic measures for partitions of External Degree Models ---

# @ edm.pyx
    
#cpdef double log2( double x )
#cpdef double xlog2x( double x )
