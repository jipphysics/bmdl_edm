# distutils: language = c++
# distutils: sources = ExternalDegreeModel/lib/cpp_graph.cpp ExternalDegreeModel/lib/cpp_edm.cpp ExternalDegreeModel/lib/cpp_nhoa.cpp

# === Cimports ===

#cimport numpy as np
#cimport cython
#from libcpp.set cimport set as cset
#from libcpp.map cimport map as cmap
from cython.operator cimport dereference as deref, preincrement as inc

# === Python wrapper for the C++ NHOA function toolboox ===
            
def PyRunNhoa( PyExternalDegreeModel edm , double beta = 0.000001 , loop_max = None , double err_tol = 0.0001 , bint verbose = True , bint one_cluster_per_node = True ):
        """
        Runs a Louvain's~[1] like optimization procedure on the External Degree Model.
        
        edm : <PyExternalDegreeModel> :: ...
        beta : <float> :: The inverse temperature at which the minimization of the effective Hamiltonian should be carried on.
        loop_max : <int> :: The maximum number of node-relabeling steps that NHOA is allowed to perform. This number is there for safety reasons, i.e. to avoid an infinite loop to exists. However, it should be chosen to be very large. Otherwise, the optimization process stops before the best result is obtained.
        err_tol : <float> :: The relative difference that should be obtained for a change in the MDL to be considered significative. This is to avoid the structure of the underlying edm to be updated because MDL became smaller due to numerical rounding error.
        verbose : <bool> :: If True, the progress of the optimization process is reported on the standard output.
        one_cluster_per_node : <bool> :: If True, the NHOA algorithm begins the optimization procedure with a configuration determined by one cluster per node.
        
        References
        ----------
        [1]  Fast unfolding of communities in large networks, Vincent D Blondel, Jean-Loup Guillaume, Renaud Lambiotte, Etienne Lefebvre, Journal of Statistical Mechanics: Theory and Experiment 2008 (10), P1000
        
        THEORY
        ------
        # TODO
        """   
        cdef bool failed_convergence = False
        if loop_max is None:
            loop_max = 10 * edm.num_nodes() * edm.num_nodes()
        best_E = run_nhoa( deref( edm._c_edm ) , beta , loop_max , err_tol , verbose , one_cluster_per_node , failed_convergence )
        if failed_convergence:
            return None
        return best_E

def PySampleNhoa( PyExternalDegreeModel edm , double beta = 0.000001 , loop_max = None , double err_tol = 0.0001 , bint verbose = True , bint one_cluster_per_node = True , int repetitions = 10 ):
        """
        Input
        -----
        Runs a Louvain's~[1] like optimization procedure on the External Degree Model.
        
        edm : <PyExternalDegreeModel> :: ...
        beta : <float> :: The inverse temperature at which the minimization of the effective Hamiltonian should be carried on.
        loop_max : <int> :: The maximum number of node-relabeling steps that NHOA is allowed to perform. This number is there for safety reasons, i.e. to avoid an infinite loop to exists. However, it should be chosen to be very large. Otherwise, the optimization process stops before the best result is obtained.
        err_tol : <float> :: The relative difference that should be obtained for a change in the MDL to be considered significative. This is to avoid the structure of the underlying edm to be updated because MDL became smaller due to numerical rounding error.
        verbose : <bool> :: If True, the progress of the optimization process is reported on the standard output.
        one_cluster_per_node : <bool> :: If True, the NHOA algorithm begins the optimization procedure with a configuration determined by one cluster per node.
        repetitions : <int> :: The number of repetitions that minimization procedure is carried on. The best of these repetitions is the one returned.

        Returns
        -------
        ( <double> , <PyExternalDegreeModel> ) : The value of the best "energy" and the corresponding configuration for the External Degree Model. Additionally, it is important to notice that the configuration of the passed External Degree Model changes during the optimization process and not necessarily ends in the best configuration.

        References
        ----------
        [1]  Fast unfolding of communities in large networks, Vincent D Blondel, Jean-Loup Guillaume, Renaud Lambiotte, Etienne Lefebvre, Journal of Statistical Mechanics: Theory and Experiment 2008 (10), P1000
        
        THEORY
        ------
        # TODO
        """
        cdef bool failed_convergence = False
        cdef PyExternalDegreeModel best_edm = edm.copy()
        if loop_max is None:
            loop_max = 10 * edm.num_nodes() * edm.num_nodes()
        best_E = sample_nhoa( deref( edm._c_edm ) , deref( best_edm._c_edm ) , beta , loop_max , err_tol , verbose , repetitions , failed_convergence )
        if failed_convergence:
            return ( None , None )
        return ( best_E , best_edm )
