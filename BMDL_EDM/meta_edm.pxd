# === Cimports ===

#cimport numpy as np
#cimport cython
from libcpp cimport bool
from libcpp.set cimport set as cset
from libcpp.map cimport map as cmap
from libcpp.pair cimport pair as cpair
from cython.operator cimport dereference as deref, preincrement as inc

# === Expossing the cython code in other modules ===

from edm cimport *
from partition cimport *

# === Expossing the C++ classes ===

cdef extern from "cpp_static_graph.h":
    cdef cppclass StaticGraph:
        StaticGraph( cset[ cpair[ int , int ] ] & links ) except +
        #~StaticGraph() except +

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

        #t_label_iterator label_it()
        #t_element_iterator  element_it( int l )


cdef extern from "cpp_edm.h":
    cdef cppclass ExternalDegreeModel:

        ctypedef Partition.t_label_iterator      t_label_iterator
        ctypedef Partition.t_element_iterator    t_labelset_node_iterator

        t_label_iterator            label_it()
        t_labelset_node_iterator    labelset_node_it( int l )

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

        Partition & node_partition()
        Partition * node_partition_ptr()

cdef extern from "cpp_runningstats.h":
   cdef cppclass RunningStats:

        RunningStats() except +
        RunningStats( const RunningStats & rs ) except +
        #~RunningStats()
        RunningStats & operator=( const RunningStats & other )

        void reset()
        void insert( double x )
        long int num_samples()
        double mean()
        double variance()
        double std()
        double std_of_mean()
        double sem()
        double rsem()
        double min()
        double max()
        double second_moment()


cdef extern from "lib/cpp_meta_edm.h":
    cdef cppclass meta_EDM:

        meta_EDM( ExternalDegreeModel & edm , double beta ) except +
        meta_EDM( meta_EDM & meta_edm ) except +
        meta_EDM & operator=( const meta_EDM & other )
        #~meta_EDM()

        void set_beta( double beta )

        ExternalDegreeModel & edm()

        void re_seed_rng()
        double xi()

        double energy()
        void run_atomic_MC_step( double meta_beta , int node , int new_label )
        void run_MC_step( double meta_beta )

        void thermalize( double meta_beta , int num_MC_steps , bool verbose )
        void linear_annealing( double meta_beta_ini , double meta_beta_end , int num_MC_steps )
        void powerlaw_annealing( double meta_beta_ini , double meta_beta_end , int max_num_MC_steps )
        void log_annealing( double meta_beta_ini , double meta_beta_end , int max_num_MC_steps )
        void integrate( double meta_beta , int num_MC_steps , Partition & ground_truth_partition , int size_partition_pool , RunningStats & rs_NMIa_gt , RunningStats & rs_NMIg_gt , RunningStats & rs_NMIm_gt , RunningStats & rs_NMImi_sg , RunningStats & rs_AMI_gt , RunningStats & rs_NMIa_sg , RunningStats & rs_NMIg_sg , RunningStats & rs_NMIm_sg , RunningStats & rs_NMImi_sg , RunningStats & rs_AMI_sg , RunningStats & rs_energy , RunningStats & rs_num_communities , bool verbose )

# === Python Wrapper for the C++ ExternalDegreeModel class ===

cdef class PyMetaEDM:

    # === Data-members ===

    cdef double                  _beta
    cdef PyExternalDegreeModel   _edm
    cdef meta_EDM *              _c_meta_edm    

    # === Function-members ===

    # @ meta_edm.pyx

# === Complements ===
    
# @ meta_edm.pyx
  
