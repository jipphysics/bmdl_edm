# === Cimports ===

#cimport numpy as np
#cimport cython
from libcpp cimport bool
from libcpp.set cimport set as cset
from libcpp.map cimport map as cmap
from libcpp.pair cimport pair as cpair
from cython.operator cimport dereference as deref, preincrement as inc

# === Expossing the cython code in other modules ===

#from graph cimport *

# === Expossing the C++ classes ===

cdef extern from "cpp_partition.h" namespace "Partition":
    cdef cppclass t_label_iterator:

        t_label_iterator() except +        
        t_label_iterator( Partition & part ) except +

        int deref()
        void inc()
        bool keepgoing()        

cdef extern from "cpp_partition.h" namespace "Partition":
    cdef cppclass t_element_iterator:

        t_element_iterator() except +        
        t_element_iterator( Partition & part ) except +

        int deref()
        void inc()
        bool keepgoing()

cdef extern from "cpp_partition.h":
    cdef cppclass Partition:

        Partition( int size ) except +
        #~Partition()

        t_label_iterator label_it()
        t_element_iterator element_it( int l )

        int  size()
        int  num_active_labels()
        int  label_count( int l )
        int  total_count()
        bool is_label_active( int l )
        int  label_of_element( int i )
        void change_label_of_element( int i , int new_l )

# === Python Wrapper for the C++ ExternalDegreeModel class ===

cdef class PyPartition:

    # === Data-members ===

    cdef int           _size
    cdef Partition *   _c_partition
    cdef object        _e_enum

    # === Function-members ===

    #cdef Partition * ptr_c_partition( self )

    # @ edm.pyx

# === Complements ===
  
# --- Plot an External Degree Model ---
  
# @ edm.pyx

# --- Compute information theoretic measures for partitions of External Degree Models ---

# @ edm.pyx
    
#cpdef double log2( double x )
#cpdef double xlog2x( double x )
