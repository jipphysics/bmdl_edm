# #

# DEPRECATED
# # distutils: language = c++
# # distutils: sources = ExternalDegreeModel/lib/cpp_graph.cpp ExternalDegreeModel/lib/cpp_edm.cpp

# === Imports ===

import networkx as nx
import numpy as np
#import itertools
#from sklearn.metrics.cluster import adjusted_mutual_info_score
import utils

# === Cimports ===

cimport numpy as np
#cimport cython
from libcpp.pair cimport pair as cpair
#from libcpp.make_pair cimport pair as cmakepair
from libcpp.set cimport set as cset
from libcpp.map cimport map as cmap
from cython.operator cimport dereference as deref, preincrement as inc

# === Python Wrapper for the C++ ExternalDegreeModel class ===

cdef class PyMetaEDM:
    """
    Implements the Meta-ExternalDegreeModel.
    """

    # === Data-members ===
    
    # @ edm.pxd
    
    # === Function-members ===    
    
    def __cinit__( self , PyExternalDegreeModel edm , beta = 0. ):
        
        self._beta = beta
        self._edm = edm
        self._c_meta_edm = new meta_EDM( deref( self._edm._c_edm ) , self._beta )

    def __dealloc__( self ):
    
        del self._c_meta_edm

    def copy( self ):

        return PyMetaEDM( self._edm , beta = self._beta )

    def __repr__( self ):
        s  = 'meta-EDM{ ' 
        s += str( self._edm ) 
        s += " ::"
        s += " beta=" + str( self._beta )
        s += ", mdl=" + str( self._edm.mdl( self._beta ) )
        s += " }"
        return s

    def edm( self ):
        
        return self._edm

    def set_beta( self , beta ):

        self._beta = beta

    def edm( self ):

        return self._edm

    def re_seed_rng( self ):
    
        self._c_meta_edm.re_seed_rng()

    def xi( self ):

        return self._c_meta_edm.xi()

    def energy( self ):

        return self._c_meta_edm.energy()

    def run_atomic_MC_step( self , meta_beta , node , new_label ):

        self._c_meta_edm.run_atomic_MC_step( meta_beta , node , new_label )
     
    def run_MC_step( self , meta_beta ):

        self._c_meta_edm.run_MC_step( meta_beta )
  
    def thermalize( self , meta_beta , num_MC_steps , verbose = False ):

        self._c_meta_edm.thermalize( meta_beta , num_MC_steps , verbose )
      
    def linear_annealing( self , meta_beta_ini , meta_beta_end , num_MC_steps ):
 
        self._c_meta_edm.linear_annealing( meta_beta_ini , meta_beta_end , num_MC_steps )

    def power_law_annealing( self , meta_beta_ini , meta_beta_end , max_num_MC_steps ):

        self._c_meta_edm.powerlaw_annealing( meta_beta_ini , meta_beta_end , max_num_MC_steps )

    def log_annealing( self , meta_beta_ini , meta_beta_end , max_num_MC_steps ):

        self._c_meta_edm.log_annealing( meta_beta_ini , meta_beta_end , max_num_MC_steps )

    def integrate( self , meta_beta , num_MC_steps , ground_truth_partition , size_partition_pool = None , verbose = False ):

        cdef PyPartition pypart_ground_truth_partition
        cdef RunningStats * ptr_rs_NMIa_gt
        cdef RunningStats * ptr_rs_NMIg_gt
        cdef RunningStats * ptr_rs_NMIm_gt
        cdef RunningStats * ptr_rs_NMImi_gt
        cdef RunningStats * ptr_rs_AMI_gt
        cdef RunningStats * ptr_rs_NMIa_sg
        cdef RunningStats * ptr_rs_NMIg_sg
        cdef RunningStats * ptr_rs_NMIm_sg
        cdef RunningStats * ptr_rs_NMImi_sg
        cdef RunningStats * ptr_rs_AMI_sg
        cdef RunningStats * ptr_rs_energy
        cdef RunningStats * ptr_rs_num_communities
        cdef int l
        cdef int ii

        ptr_rs_NMIa_gt = new RunningStats()
        ptr_rs_NMIg_gt = new RunningStats()
        ptr_rs_NMIm_gt = new RunningStats()
        ptr_rs_NMImi_gt = new RunningStats()
        ptr_rs_AMI_gt = new RunningStats()
        ptr_rs_NMIa_sg = new RunningStats()
        ptr_rs_NMIg_sg = new RunningStats()
        ptr_rs_NMIm_sg = new RunningStats()
        ptr_rs_NMImi_sg = new RunningStats()
        ptr_rs_AMI_sg = new RunningStats()
        ptr_rs_energy = new RunningStats()
        ptr_rs_num_communities = new RunningStats()

        if not isinstance( ground_truth_partition , PyPartition ):
            try:
                pypart_ground_truth_partition = PyPartition( ground_truth_partition )
            except:
                assert False , 'ERROR @ PyMetaEDM.integrate : unkown format for "ground_truth_partition".'
        else:
            pypart_ground_truth_partition = ground_truth_partition

        if size_partition_pool is None:
            size_partition_pool = self._edm._c_edm.G().num_nodes()

        self._c_meta_edm.integrate( meta_beta , num_MC_steps , deref( pypart_ground_truth_partition._c_partition ) , size_partition_pool , deref( ptr_rs_NMIa_gt ) , deref( ptr_rs_NMIg_gt ) , deref( ptr_rs_NMIm_gt ) , deref( ptr_rs_NMImi_gt ) , deref( ptr_rs_AMI_gt ) , deref( ptr_rs_NMIa_sg ) , deref( ptr_rs_NMIg_sg ) , deref( ptr_rs_NMIm_sg ) , deref( ptr_rs_NMImi_sg ) , deref( ptr_rs_AMI_sg ) , deref( ptr_rs_energy ) , deref( ptr_rs_num_communities ) , verbose )

        rs_NMIa_gt = {}
        rs_NMIg_gt = {}
        rs_NMIm_gt = {}
        rs_NMImi_gt = {}
        rs_AMI_gt = {}
        rs_NMIa_sg = {}
        rs_NMIg_sg = {}
        rs_NMIm_sg = {}
        rs_NMImi_sg = {}
        rs_AMI_sg = {}
        rs_energy = {}
        rs_num_communities = {}

        rs_NMIa_gt[ 'mean' ] = ptr_rs_NMIa_gt.mean()
        rs_NMIg_gt[ 'mean' ] = ptr_rs_NMIg_gt.mean()
        rs_NMIm_gt[ 'mean' ] = ptr_rs_NMIm_gt.mean()
        rs_NMImi_gt[ 'mean' ] = ptr_rs_NMImi_gt.mean()
        rs_AMI_gt[ 'mean' ] = ptr_rs_AMI_gt.mean()
        rs_NMIa_sg[ 'mean' ] = ptr_rs_NMIa_sg.mean()
        rs_NMIg_sg[ 'mean' ] = ptr_rs_NMIg_sg.mean()
        rs_NMIm_sg[ 'mean' ] = ptr_rs_NMIm_sg.mean()
        rs_NMImi_sg[ 'mean' ] = ptr_rs_NMImi_sg.mean()
        rs_AMI_sg[ 'mean' ] = ptr_rs_AMI_sg.mean()
        rs_energy[ 'mean' ] = ptr_rs_energy.mean()
        rs_num_communities[ 'mean' ] = ptr_rs_num_communities.mean()

        rs_NMIa_gt[ 'std' ] = ptr_rs_NMIa_gt.std()
        rs_NMIg_gt[ 'std' ] = ptr_rs_NMIg_gt.std()
        rs_NMIm_gt[ 'std' ] = ptr_rs_NMIm_gt.std()
        rs_NMImi_gt[ 'std' ] = ptr_rs_NMImi_gt.std()
        rs_AMI_gt[ 'std' ] = ptr_rs_AMI_gt.std()
        rs_NMIa_sg[ 'std' ] = ptr_rs_NMIa_sg.std()
        rs_NMIg_sg[ 'std' ] = ptr_rs_NMIg_sg.std()
        rs_NMIm_sg[ 'std' ] = ptr_rs_NMIm_sg.std()
        rs_NMImi_sg[ 'std' ] = ptr_rs_NMImi_sg.std()
        rs_AMI_sg[ 'std' ] = ptr_rs_AMI_sg.std()
        rs_energy[ 'std' ] = ptr_rs_energy.std()
        rs_num_communities[ 'std' ] = ptr_rs_num_communities.std()

        rs_NMIa_gt[ 'sem' ] = ptr_rs_NMIa_gt.sem()
        rs_NMIg_gt[ 'sem' ] = ptr_rs_NMIg_gt.sem()
        rs_NMIm_gt[ 'sem' ] = ptr_rs_NMIm_gt.sem()
        rs_NMImi_gt[ 'sem' ] = ptr_rs_NMImi_gt.sem()
        rs_AMI_gt[ 'sem' ] = ptr_rs_AMI_gt.sem()
        rs_NMIa_sg[ 'sem' ] = ptr_rs_NMIa_sg.sem()
        rs_NMIg_sg[ 'sem' ] = ptr_rs_NMIg_sg.sem()
        rs_NMIm_sg[ 'sem' ] = ptr_rs_NMIm_sg.sem()
        rs_NMImi_sg[ 'sem' ] = ptr_rs_NMImi_sg.sem()
        rs_AMI_sg[ 'sem' ] = ptr_rs_AMI_sg.sem()
        rs_energy[ 'sem' ] = ptr_rs_energy.sem()
        rs_num_communities[ 'sem' ] = ptr_rs_num_communities.sem()

        rs_NMIa_gt[ 'rsem' ] = ptr_rs_NMIa_gt.rsem()
        rs_NMIg_gt[ 'rsem' ] = ptr_rs_NMIg_gt.rsem()
        rs_NMIm_gt[ 'rsem' ] = ptr_rs_NMIm_gt.rsem()
        rs_NMImi_gt[ 'rsem' ] = ptr_rs_NMImi_gt.rsem()
        rs_AMI_gt[ 'rsem' ] = ptr_rs_AMI_gt.rsem()
        rs_NMIa_sg[ 'rsem' ] = ptr_rs_NMIa_sg.rsem()
        rs_NMIg_sg[ 'rsem' ] = ptr_rs_NMIg_sg.rsem()
        rs_NMIm_sg[ 'rsem' ] = ptr_rs_NMIm_sg.rsem()
        rs_NMImi_sg[ 'rsem' ] = ptr_rs_NMImi_sg.rsem()
        rs_AMI_sg[ 'rsem' ] = ptr_rs_AMI_sg.rsem()
        rs_energy[ 'rsem' ] = ptr_rs_energy.rsem()
        rs_num_communities[ 'rsem' ] = ptr_rs_num_communities.rsem()

        rs_NMIa_gt[ 'min' ] = ptr_rs_NMIa_gt.min()
        rs_NMIg_gt[ 'min' ] = ptr_rs_NMIg_gt.min()
        rs_NMIm_gt[ 'min' ] = ptr_rs_NMIm_gt.min()
        rs_NMImi_gt[ 'min' ] = ptr_rs_NMImi_gt.min()
        rs_AMI_gt[ 'min' ] = ptr_rs_AMI_gt.min()
        rs_NMIa_sg[ 'min' ] = ptr_rs_NMIa_sg.min()
        rs_NMIg_sg[ 'min' ] = ptr_rs_NMIg_sg.min()
        rs_NMIm_sg[ 'min' ] = ptr_rs_NMIm_sg.min()
        rs_NMImi_sg[ 'min' ] = ptr_rs_NMImi_sg.min()
        rs_AMI_sg[ 'min' ] = ptr_rs_AMI_sg.min()
        rs_energy[ 'min' ] = ptr_rs_energy.min()
        rs_num_communities[ 'min' ] = ptr_rs_num_communities.min()

        rs_NMIa_gt[ 'max' ] = ptr_rs_NMIa_gt.max()
        rs_NMIg_gt[ 'max' ] = ptr_rs_NMIg_gt.max()
        rs_NMIm_gt[ 'max' ] = ptr_rs_NMIm_gt.max()
        rs_NMImi_gt[ 'max' ] = ptr_rs_NMImi_gt.max()
        rs_AMI_gt[ 'max' ] = ptr_rs_AMI_gt.max()
        rs_NMIa_sg[ 'max' ] = ptr_rs_NMIa_sg.max()
        rs_NMIg_sg[ 'max' ] = ptr_rs_NMIg_sg.max()
        rs_NMIm_sg[ 'max' ] = ptr_rs_NMIm_sg.max()
        rs_NMImi_sg[ 'max' ] = ptr_rs_NMImi_sg.max()
        rs_AMI_sg[ 'max' ] = ptr_rs_AMI_sg.max()
        rs_energy[ 'max' ] = ptr_rs_energy.max()
        rs_num_communities[ 'max' ] = ptr_rs_num_communities.max()

        rs_NMIa_gt[ 'num_samples' ] = ptr_rs_NMIa_gt.num_samples()
        rs_NMIg_gt[ 'num_samples' ] = ptr_rs_NMIg_gt.num_samples()
        rs_NMIm_gt[ 'num_samples' ] = ptr_rs_NMIm_gt.num_samples()
        rs_NMImi_gt[ 'num_samples' ] = ptr_rs_NMImi_gt.num_samples()
        rs_AMI_gt[ 'num_samples' ] = ptr_rs_AMI_gt.num_samples()
        rs_NMIa_sg[ 'num_samples' ] = ptr_rs_NMIa_sg.num_samples()
        rs_NMIg_sg[ 'num_samples' ] = ptr_rs_NMIg_sg.num_samples()
        rs_NMIm_sg[ 'num_samples' ] = ptr_rs_NMIm_sg.num_samples()
        rs_NMImi_sg[ 'num_samples' ] = ptr_rs_NMImi_sg.num_samples()
        rs_AMI_sg[ 'num_samples' ] = ptr_rs_AMI_sg.num_samples()
        rs_energy[ 'num_samples' ] = ptr_rs_energy.num_samples()
        rs_num_communities[ 'num_samples' ] = ptr_rs_num_communities.num_samples()

        del ptr_rs_num_communities
        del ptr_rs_energy
        del ptr_rs_AMI_sg
        del ptr_rs_NMIm_sg
        del ptr_rs_NMImi_sg
        del ptr_rs_NMIg_sg
        del ptr_rs_NMIa_sg
        del ptr_rs_AMI_gt
        del ptr_rs_NMIm_gt
        del ptr_rs_NMImi_gt
        del ptr_rs_NMIg_gt
        del ptr_rs_NMIa_gt
        
        return rs_NMIa_gt , rs_NMIg_gt , rs_NMIm_gt , rs_NMImi_gt , rs_AMI_gt , rs_NMIa_sg , rs_NMIg_sg , rs_NMIm_sg , rs_NMImi_sg , rs_AMI_sg , rs_energy , rs_num_communities

# === Complements ===
