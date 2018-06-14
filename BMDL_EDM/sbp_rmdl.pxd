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

from edm cimport *
#from partition cimport *

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
        int  size()
        int  num_active_labels()
        int  label_count( int l )
        int  total_count()
        bool is_label_active( int l )
        int  label_of_element( int i )
        void change_label_of_element( int i , int new_l )
        t_label_iterator label_it()
        t_element_iterator  element_it( int l )


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
        StaticGraph( cset[ cpair[ int , int ] ] & links ) except +
        #~StaticGraph() except +
        int num_nodes()
        int num_links()
        int degree( int i )
        t_nn_iterator nn_it( int i )

cdef extern from "cpp_edm.h":
    cdef cppclass ExternalDegreeModel:
        #ctypedef Partition.t_label_iterator      t_label_iterator
        #ctypedef Partition.t_element_iterator    t_labelset_node_iterator

        #t_label_iterator            label_it()
        #t_labelset_node_iterator    labelset_node_it( int l )
        Partition.t_label_iterator    label_it()
        Partition.t_element_iterator  labelset_node_it( int l )

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

        double j_2_i( int i )
        double H()

        double j_1()
        double j_2()

        double j( double beta )
        double J( double beta )

        double ln_modA()

        double mdl( double beta )

        void set_one_label_per_node()
        void set_one_label_all_nodes()

        #Partition & node_partition()
        #Partition * node_partition_ptr()

cdef extern from "cpp_sbp_rmdl.h":
    cdef cppclass t_BP_summary:
        bool        converged
        double      max_err
        int         run_MC_steps
        #bool        paramagnetic

cdef extern from "cpp_sbp_rmdl.h":
    cdef cppclass SBP_RMDL:
        # Notice, the beta here corresponds to the meta_beta in the RMDL formulation.
        SBP_RMDL( StaticGraph & I , int num_labels , double beta ) except +
        SBP_RMDL( SBP_RMDL & gm ) except +
        SBP_RMDL & operator=( const SBP_RMDL & other )
        #~SBP_RMDL()

        StaticGraph & I()
        int num_variables()
        int num_factors()
        int num_labels()
        double beta()
        double T()
        #void normalize_n( int l )
        #void normalize_b( int i )
        #void set_uniform_n()
        #void set_uniform_b()
        #void set_random_n()
        #void set_random_b()
        #void update_theta_and_eta()
        void set_paramagnetic_state()
        #double run_BP_step( StaticGraph::t_nn_iterator nn_it , double dumping , double paramagnetic_tol , int verbose )
        t_BP_summary run_BP_fast( int MC_steps , double err_tol , double dumping , double paramagnetic_tol , int verbose )
        cvector[ double ] & b_i( int i )
        void compute_b_ij( int i , int j , cvector[ cvector[ double ] ] & b_ij )
        int s_MAP( int i )
        double H_i( int i , int s_i )
        double H_ij( int i , int j , int s_i , int s_j )
        void compute_thermostatistics()
        double energy()
        double entropy()
        double free_energy()
        double paramagnetic_free_energy()
        double paramagnetic_energy()
        double paramagnetic_entropy()
        double ln_Z()
        double hamiltonian( cvector[ int ] & s )
        double RMDL( cvector[ int ] & s )


# === Python Wrapper for the C++ classes and functions ===

cdef class Py_SBP_RMDL:

    # === Data-members ===

    cdef PyExternalDegreeModel   _edm
    cdef SBP_RMDL *              _c_sbp_rmdl

    # === Function-members ===

    # @ SBP_RMDL.pyx

# === Complements ===
    
# @ SBP_RMDL.pyx  
