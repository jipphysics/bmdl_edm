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

cdef class Py_SBP_MOD:
    """
    Implements the Sparse Belief-Propagation engine for a graphical model based on the Girvan-Newman modularity. 
    This is an implementation of the so called MOD-BP~[1].

    Refs.
    =====
    [1] Zhang, Pan, and Cristopher Moore. "Scalable detection of statistically significant communities and hierarchies, using message passing for modularity." Proceedings of the National Academy of Sciences 111.51 (2014): 18144-18149.
    [2] http://lib.itp.ac.cn/html/panzhang/mod/mod.tar.gz
    """

    # === Data-members ===
    
    # @ sbp_mod.pxd
    
    # === Function-members ===    
    
    def __cinit__( self , PyExternalDegreeModel edm , num_labels , meta_beta = 1. ):
        
        self._edm = edm
        self._c_sbp_mod = new SBP_MOD( deref( self._edm._c_edm.G_ptr() ) , num_labels , meta_beta )

    def __dealloc__( self ):
    
        del self._c_sbp_mod

    def copy( self ):

        return Py_SBP_MOD( self._edm , self.num_labels() , meta_beta = self.meta_beta() )

    def __repr__( self ):
        cdef int _i
        cdef int _j
        cdef int _s
        cdef cvector[ double ] b_i
        cdef cvector[ cvector[ double ] ] b_ij

        s  = 'Py_SBP_MOD{\n' 
        s += "    num_labels=" + str( self.num_labels() ) + "\n"
        s += "    meta_beta=" + str( self.meta_beta() ) + "\n"
        s += "    energy=" + str( self.energy() ) + "\n"
        s += "    entropy=" + str( self.entropy() ) + "\n"
        s += "    free_energy=" + str( self.free_energy() ) + "\n"
        s += "    s_MAPs and Beliefs:\n"
        for i in self.var_it():
            _i = self._edm._node_enum.l2i( i )
            b_i = self._c_sbp_mod.b_i( _i )
            s += "        s_MAP[ " + str( i ) + " ]=" + str( self.s_MAP( i ) ) + "\n"
            for _s in self.label_it():
                s += "            b_{ " + str( i ) + " }[ " + str( _s ) + " ]=" + str( b_i[ _s ] ) + "\n"
        s += "}"
        return s

    def edm( self ):
        
        return self._edm
   
    def num_variables( self ):

        return self._c_sbp_mod.num_variables()

    def num_factors( self ):

        return self._c_sbp_mod.num_factors()

    def num_labels( self ):

        return self._c_sbp_mod.num_labels()

    def meta_beta( self ):

        # Remember, meta_beta for us is beta for MOD-BP according to the original formulation.
        return self._c_sbp_mod.beta()        

    def meta_T( self ):

        # Remember, meta_beta for us is beta for MOD-BP according to the original formulation.
        return self._c_sbp_mod.T()

    def var_it( self ):

        #for i in self._edm.it_nodes():
        #    print "TEST @ Py_SBP_MOD.var_it : i =" , i
        #    yield i
        return self._edm.it_nodes()

    def factor_it( self ):
        cdef int _i
        cdef int _j
        cdef StaticGraph.t_nn_iterator _nn_it
        #cdef SBP_MOD.t_static_graph_nn_iterator _nn_it
        #cdef t_static_graph_nn_iterator _nn_it

        for _i in xrange( self._c_sbp_mod.num_variables() ):
            _nn_it = self._c_sbp_mod.I().nn_it( _i )
            while _nn_it.keepgoing():
                _j = _nn_it.j()
                _nn_it.inc()
                if _i < _j:
                    i = self._edm._node_enum.i2l( _i )
                    j = self._edm._node_enum.i2l( _j )
                    yield ( i , j )

    def label_it( self ):

        for s in xrange( self.num_labels() ):
            yield s

    def run_BP_fast( self , MC_steps = None , err_tol = 0.000001 , dumping = 0.5 , paramagnetic_tol = 0.001 , verbose = -1 ):

        cdef int _i
        cdef int _s
        cdef int run_MC_steps
        cdef t_BP_summary BP_summary

        if MC_steps is None:
            MC_steps = int( 36 * 2 * self._edm.num_links() / ( 0.000001 + dumping ) )
        assert MC_steps > 0
        assert err_tol > 0. and err_tol < 1.
        assert dumping > 0. and dumping < 1.
        assert verbose >= -1

        BP_summary = self._c_sbp_mod.run_BP_fast( MC_steps , err_tol , dumping , paramagnetic_tol , verbose )

        self.set_MAP()

        result = {}
        result[ 'converged' ]    = BP_summary.converged
        result[ 'max_err' ]      = BP_summary.max_err
        result[ 'run_MC_steps' ] = BP_summary.run_MC_steps
        #result[ 'paramagnetic' ] = BP_summary.paramagnetic
        return result

    def set_MAP( self ):

        for i in self._edm.it_nodes():
            _i = self._edm._node_enum.l2i( i )
            _s = self._c_sbp_mod.s_MAP( _i )
            # FOR-TESTING
            #print "TEST @ Py_SBP_MOD.run_BP_fast : _i , _s =" , _i , _s
            self._edm.set_node_label( i , _s )

    def energy( self ):

        return self._c_sbp_mod.energy()

    def entropy( self ):

        return self._c_sbp_mod.entropy()

    def free_energy( self ):

        return self._c_sbp_mod.free_energy()

    def paramagnetic_energy( self ):

        return self._c_sbp_mod.paramagnetic_energy()

    def paramagnetic_entropy( self ):

        return self._c_sbp_mod.paramagnetic_entropy()

    def paramagnetic_free_energy( self ):

        return self._c_sbp_mod.paramagnetic_free_energy()

    def ln_Z( self ):
    
        return self._c_sbp_mod.ln_Z()

    def meta_hamiltonian( self , states = None ):
        cdef cvector[ int ] cvec_s
        cdef int _i
        cdef int _s

        cvec_s.resize( self._c_sbp_mod.num_variables() )
        if states is None:
            for _i in xrange( self._c_sbp_mod.num_variables() ):
                cvec_s[ _i ] = self._c_sbp_mod.s_MAP( _i )
        else:
            assert isinstance( states , dict )
            enum_s = utils.Enumerate()
            for i in states.keys():
                _i = self._edm._node_enum.l2i( i )
                _s = enum_s.insert( states[ i ] )
                assert _s < self._c_sbp_mod.num_labels()
                cvec_s[ _i ] = states[ i ]
        return self._c_sbp_mod.hamiltonian( cvec_s )

    def meta_RMDL( self , states = None ):
        cdef cvector[ int ] cvec_s
        cdef int _i
        cdef int _s

        cvec_s.resize( self._c_sbp_mod.num_variables() )
        if states is None:
            for _i in xrange( self._c_sbp_mod.num_variables() ):
                cvec_s[ _i ] = self._c_sbp_mod.s_MAP( _i )
        else:
            assert isinstance( states , dict )
            enum_s = utils.Enumerate()
            for i in states.keys():
                _i = self._edm._node_enum.l2i( i )
                _s = enum_s.insert( states[ i ] )
                assert _s < self._c_sbp_mod.num_labels()
                cvec_s[ _i ] = states[ i ]
        return self._c_sbp_mod.RMDL( cvec_s )

    def s_MAP( self , i ):
        cdef int _i

        _i = self._edm._node_enum.l2i( i )
        return self._c_sbp_mod.s_MAP( _i )

    def states_MAP( self ):

        return { i : self.s_MAP( i ) for i in self.var_it() }


    def relative_free_energy( self , tol = 0.0 ):
        """
        INPUT
        tol : <float> :: TODO
        rel_diff_tol : <float> :: TODO

        Let:
            F = free_energy
            F0 = paramagnetic_free_energy
            eps = rel_diff_tol
            rd = 2 * ( F0 - F ) / ( | F0 | + | F | + eps )

        RETURN
        rd < tol        
        """        
        return utils.rel_diff( self.paramagnetic_free_energy() , self.free_energy() , tol = tol )

    def is_paramagnetic( self , tol = 0.01 ,  rel_diff_tol = 0.0 ):
        """
        INPUT
        tol : <float> :: TODO
        rel_diff_tol : <float> :: TODO

        Let:
            F = free_energy
            F0 = paramagnetic_free_energy
            eps = rel_diff_tol
            rd = 2 * ( F0 - F ) / ( | F0 | + | F | + eps )

        RETURN
        rd < tol        
        """        
        return abs( self.relative_free_energy( tol = rel_diff_tol ) ) < tol

# === Complements ===
