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

cdef extern from "cpp_static_graph.h" namespace "StaticGraph":
    cdef cppclass t_nn_iterator:
        t_nn_iterator() except +
        t_nn_iterator( StaticGraph & G , int i ) except +
        t_nn_iterator( t_nn_iterator & n ) except +
        int i() 
        int j()
        int l()
        bool keepgoing()
        void inc()

cdef extern from "cpp_static_graph.h":
    cdef cppclass StaticGraph:
        StaticGraph( int num_nodes , cset[ cpair[ int , int ] ] & links ) except +
        #~StaticGraph() except +
        int num_nodes()
        int num_links()
        int degree( int i )
        t_nn_iterator nn_it( int i )

cdef extern from "cpp_partition.h":
    cdef cppclass Partition:
        Partition( int size ) except +
        #~Partition()

cdef extern from "cpp_partition.h" namespace "Partition":
    cppclass t_label_iterator:
        t_label_iterator() except +        
        t_label_iterator( Partition & part ) except +
        int deref()
        void inc()
        bool keepgoing()        
    cppclass t_element_iterator:
        t_element_iterator() except +        
        t_element_iterator( Partition & part ) except +
        int deref()
        void inc()
        bool keepgoing()

cdef extern from "cpp_edm.h":
    cdef cppclass ExternalDegreeModel:
        #ctypedef Partition.t_label_iterator      t_label_iterator
        #ctypedef Partition.t_element_iterator    t_labelset_node_iterator

        #t_label_iterator            label_it()
        #t_labelset_node_iterator    labelset_node_it( int l )

        Partition.t_label_iterator      label_it()
        Partition.t_element_iterator    labelset_node_it( int l )


        ExternalDegreeModel( StaticGraph & G )
        ExternalDegreeModel( const ExternalDegreeModel & edm )
        #~ExternalDegreeModel()

        ExternalDegreeModel & operator=( const ExternalDegreeModel & other )

        const StaticGraph & G()
        const StaticGraph * G_ptr()

        #bool trivial_G()

        void update_quantities()

        int get_node_label( int i )
        void set_node_label( int i , int new_l )
 
        bool has_label( int l )
        int num_active_labels()

        double total_kappa()
        double kappa( int l )

        int num_intra_links()
        int num_inter_links()

        double avrg_num_intra_links()
        double avrg_num_inter_links()

        double degree( int i )

        double external_degree( int i )
        double internal_degree( int i )

        double avrg_external_degree( int i )
        double avrg_internal_degree( int i )

        double effective_Hamiltonian_ord1_i( int i )
        double Hamiltonian()

        double effective_Hamiltonian_ord0()
        double effective_Hamiltonian_ord1()

        double effective_Hamiltonian( double beta )
        double effective_L( double beta )

        double ln_modA()

        double mdl( double beta )

        void set_one_label_per_node()
        void set_one_label_all_nodes()

        Partition & node_partition()
        Partition * node_partition_ptr()

# === Python Wrapper for the C++ ExternalDegreeModel class ===

cdef class PyExternalDegreeModel:

    # === Data-members ===

    cdef object                  _G
    cdef object                  _node_enum
    #cdef object                  _label_enum
    cdef StaticGraph *           _c_static_graph
    cdef ExternalDegreeModel *   _c_edm    

    # === Function-members ===

    # @ edm.pyx

# === Complements ===
  
# --- Plot an External Degree Model ---
  
# @ edm.pyx

# --- Compute information theoretic measures for partitions of External Degree Models ---

# @ edm.pyx
    
#cpdef double log2( double x )
#cpdef double xlog2x( double x )
