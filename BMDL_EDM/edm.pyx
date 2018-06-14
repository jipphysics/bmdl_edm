# distutils: language = c++

# DEPRECATED
# # # distutils: sources = ExternalDegreeModel/lib/cpp_graph.cpp ExternalDegreeModel/lib/cpp_edm.cpp

# === Imports ===

import networkx as nx
import numpy as np
import itertools
from sklearn.metrics.cluster import adjusted_mutual_info_score
import utils
from partition import PyPartition

#from graph import PyGraph_to_nxGraph

# === Cimports ===

cimport numpy as np
#cimport cython
from libcpp.pair cimport pair as cpair
#from libcpp.make_pair cimport make_pair as cmakepair
from libcpp.set cimport set as cset
from libcpp.map cimport map as cmap
from cython.operator cimport dereference as deref, preincrement as inc

# === Python Wrapper for the C++ ExternalDegreeModel class ===

cdef class PyExternalDegreeModel:
    """
    Implements the ExternalDegreeModel.
    """

    # === Data-members ===
    
    # @ edm.pxd
    
    # === Function-members ===    
    
    def __cinit__( self , G , list_of_list_partition = None ):
        """The partition should be specified as a list of list, or analogous. For example [['a','b'],['c'],['d','e']]."""        

        cdef cset[ cpair[ int , int ] ] c_links
        cdef cpair[ int , int ] c_p

        assert( G , nx.Graph )
        self._G = G

        self._node_enum = utils.Enumerate()

        # We also introduce self links---which later are ignored as links---to eventually acount for the appropriate number of nodes.
        #for i in self._G.nodes_iter(): # It seems networkx's new version uses a different set of function names.
        for i in self._G.nodes():
            self._node_enum.insert( i )

        for i , j in self._G.edges():
            c_p.first  = self._node_enum[ i ]
            c_p.second = self._node_enum[ j ]
            if c_p.first == c_p.second:
                continue 
            c_links.insert( c_p )

        self._c_static_graph = new StaticGraph( self.num_nodes() , c_links )
        self._c_edm          = new ExternalDegreeModel( deref( self._c_static_graph ) )

        if list_of_list_partition is None:
            for l , i in enumerate( self.it_nodes() ):
                self.set_node_label( i , l )
        else:
            for l , part in enumerate( list_of_list_partition ):
                for i in part:
                    self.set_node_label( i , l )

    def __dealloc__( self ):
    
        del self._c_edm
        del self._c_static_graph

    def copy( self ):

        return PyExternalDegreeModel( self._G , list_of_list_partition = self.partition()  )

    def __repr__( self ):
        s = 'EDM{'
        sep = " "
        for l in self.it_labels():
            s += sep + str( l ) + ":"
            for i in self.it_nodes( l ):
                s += " " + str( i )
            sep = " | "
        return s + " }::H_0=" + str( self.effective_Hamiltonian_ord0() ) + "::H_1=" + str( self.effective_Hamiltonian_ord1() )
      
    def G( self ):
        
        return self._G       

    def update_quantities( self ):
    
        self._c_edm.update_quantities()

    def get_node_label( self , i ):
        """Returns the label of node "i"."""
        cdef int _i

        _i = self._node_enum.l2i( i )
        return self._c_edm.get_node_label( _i )
        
    def set_node_label( self , i , int l ):
        """Notice, labels can be integers in the range {0,1,...,N-1}, only, where N is the number of nodes."""
        cdef int _i
        assert l >= 0 and l < self._c_static_graph.num_nodes() , 'ERROR @ PyExternalDegree.set_node_label : label "l" is out of valid range.'
        _i = self._node_enum.l2i( i )
        self._c_edm.set_node_label( _i , l )

    def has_label( self , int l ):
        """Notice, labels can be integers in the range {0,1,...,N-1}, only, where N is the number of nodes."""
        assert l >= 0 and l < self._c_static_graph.num_nodes() , 'ERROR @ PyExternalDegree.has_label : label "l" is out of valid range.'
        return self._c_edm.has_label( l )

    def num_nodes( self ):
   
        return self._G.number_of_nodes()

    def num_links( self ):
   
        return self._G.number_of_edges()

    def num_labels( self ):
    
        return self._c_edm.num_active_labels()        

    def node_enum( self , i ):
        """Returns the integer number which is internally assigned to node "i" during the enumeration of the nodes. """
        return self._node_enum.l2i( i )

    def total_kappa( self ):

        return self._c_edm.total_kappa()

    def kappa( self , int l ):
        """Notice, labels can be integers in the range {0,1,...,N-1}, only, where N is the number of nodes."""
        assert l >= 0 and l < self._c_static_graph.num_nodes() , 'ERROR @ PyExternalDegree.kappa : label "l" is out of valid range.'   
        return self._c_edm.kappa( l )

    def num_intra_links( self ):
    
        return self._c_edm.num_intra_links()    

    def num_inter_links( self ):
    
        return self._c_edm.num_inter_links()    

    def avrg_num_intra_links( self ):
    
        return self._c_edm.avrg_num_intra_links()    

    def avrg_num_inter_links( self ):
    
        return self._c_edm.avrg_num_inter_links()    

    def degree( self , i ):
    
        return self._c_edm.degree( self._node_enum[ i ] )

    def external_degree( self , i ):

        return self._c_edm.external_degree( self._node_enum[ i ] )
                    
    def internal_degree( self , i ):

        return self._c_edm.internal_degree( self._node_enum[ i ] )

    def avrg_external_degree( self , i ):

        return self._c_edm.avrg_external_degree( self._node_enum[ i ] )

    def avrg_internal_degree( self , i ):

        return self._c_edm.avrg_internal_degree( self._node_enum[ i ] )

    def effective_Hamiltonian_ord1_i( self , i ):

        return self._c_edm.effective_Hamiltonian_ord1_i( self._node_enum[ i ] )

    def Hamiltonian( self ):

        return self._c_edm.Hamiltonian()

    def effective_Hamiltonian_ord0( self ):
    
        return self._c_edm.effective_Hamiltonian_ord0()

    def effective_Hamiltonian_ord1( self ):
    
        return self._c_edm.effective_Hamiltonian_ord1()

    def effective_Hamiltonian( self , beta ):

        return self._c_edm.effective_Hamiltonian( beta )

    def effective_L( self , beta ):
    
        return self._c_edm.effective_L( beta )

    def ln_modA( self ):

        return self._c_edm.ln_modA()

    def mdl( self , beta ):

        return self._c_edm.mdl( beta )
               
    def partition( self ):
        """This returns the partition of the present ExternalDegreeModel in the form of a list of lists."""
        return [ [ i for i in self.it_nodes( l ) ] for l in self.it_labels() ]
        
    def pypartition( self ):

        return PyPartition( self.partition() )

    def it_labels( self ):
        #cdef ExternalDegreeModel.t_label_iterator l_it = self._c_edm.label_it()
        cdef Partition.t_label_iterator l_it
        cdef int l

        l_it = self._c_edm.label_it()
        while l_it.keepgoing():
            l = l_it.deref()
            l_it.inc()
            yield l

    # FOR TESTING
    #def it_nn( self ):
    #    cdef int j
    #    cdef StaticGraph.t_nn_iterator nn_it
    #
    #    nn_it = self._c_edm.G().nn_it( 0 )
    #    while nn_it.keepgoing():
    #        j = nn_it.j()
    #        nn_it.inc()
    #        yield j

    def it_nodes( self , l = None ):
        """Iterates over the nodes associated to label l, if l is specified. Otherwise, it iterates over all nodes. 
        Notice, labels can be integers in the range {0,1,...,N-1}, only, where N is the number of nodes."""

        #cdef ExternalDegreeModel.t_labelset_node_iterator j_it
        cdef Partition.t_element_iterator j_it
        cdef int j
        
        if l is None:
            for i in self._node_enum:
                yield i
        else:
            assert isinstance(  l , int ) , 'ERROR @ PyExternalDegree.it_nodes : label "l" is not an integer.'
            assert l >= 0 and l < self._c_static_graph.num_nodes() , 'ERROR @ PyExternalDegree.it_nodes : label "l" is out of valid range.'
            j_it = self._c_edm.labelset_node_it( l )
            while j_it.keepgoing():
                j = j_it.deref()
                j_it.inc()
                yield self._node_enum.i2l( j )

    def set_one_label_per_node( self ):
        
        self._c_edm.set_one_label_per_node()

    def set_partition( self , list_of_list_partition ):
        """The partition should be specified as a list of list, or analogous. For example [['a','b'],['c'],['d','e']]."""
        for l , part in enumerate( list_of_list_partition ):
            for i in part:
                self.set_node_label( i , l )

    def set_random_label_partition( self , num_labels = None ):
        """
        Set the nodes-labels corresponding to a random partition by choosing labels from an uniform distribution in [0,N-1] where N is specified or is the number of nodes of graph G, otherwise. Notice, the resulting number of different labels in the partition is generally close but still smaller than N since repeated labels are likely to appear during the drawing process. N cannot be larger than the number of nodes in the graph G. In essence, this function is for testing purposes, mostly.
        """
        cdef int l_rand

        if num_labels is None:
            num_labels = self.num_nodes()
        assert num_labels <= self.num_nodes() , 'ERROR @ PyExternalDegreeModel.set_random_label_partition : "num_labels" cannot be larger than the number of nodes.'
        for i in self.it_nodes():
            l_rand = np.random.randint( num_labels )
            self.set_node_label( i , l_rand )

# === Complements ===

# --- Plot an external Degree Model ---

#def plot_PyExternalDegreeModel_as_nxGraph( PyExternalDegreeModel edm , cmap = None , nodes_positions = None , intra_module_link_weight = None , k = None , iterations = 100 , with_node_labels = True , nodes_labels = None , nodes_colors = None ):
#    """
#    WARNING: This assumes that matplotlib has been activated via the (IPython 
#    or Jupyter) commands:
#    
#        %matplotlib inline
#
#        import matplotlib
#        import matplotlib.pyplot as plt
#    """
#    
#    G = edm.G()
#    nxG = PyGraph_to_nxGraph( G )
#    if nodes_colors is None:
#        print '# edm.plot_PyExternalDegreeModel_as_nxGraph : using community labels as nodes_colors...'
#        nodes_colors = [ edm.get_node_label( i ) for i in nxG.nodes() ]
#    else:
#        print '# edm.plot_PyExternalDegreeModel_as_nxGraph : using customized nodes_colors...'
#        if isinstance( nodes_colors , dict ):
#            nodes_colors = [ nodes_colors[ i ] for i in nxG.nodes() ]
#        else:
#            assert isinstance( nodes_colors , list ) , 'ERROR @ edm.plot_PyExternalDegreeModel_as_nxGraph() : assert isinstance( nodes_colors , list )'
#    if intra_module_link_weight is not None:
#        weight = 'weight'
#        def edge_weight( i , j , edm ):
#            if edm.get_node_label( i ) == edm.get_node_label( j ):
#                return float( intra_module_link_weight )
#            else:
#                return 1.
#        nx.set_edge_attributes( nxG , 'weight', { ( i , j ) : edge_weight( i , j , edm ) for i , j in nxG.edges() } )          
#        #print '# Weighted edges = ' , nxG.edges( data = True )      
#    else:
#        weight = None
#    if nodes_positions is None:
#        nodes_positions = nx.spring_layout( nxG , weight = weight , k = k , iterations = iterations )
#    if with_node_labels:
#        if nodes_labels is None:
#            nodes_labels = { i : str( i ) for i in nxG.nodes() }       
#    else:
#        nodes_labels = None
#    #comm_labels = [ l for l in edm.it_labels() ]        
#    #vmin = min( comm_labels )
#    #vmax = max( comm_labels )    
#    vmin = min( nodes_colors )
#    vmax = max( nodes_colors )
#    nx.draw_networkx( nxG ,
#                      pos = nodes_positions ,
#                      node_color = nodes_colors ,
#                      cmap = cmap ,
#                      vmin = vmin ,
#                      vmax = vmax ,
#                      with_labels = with_node_labels ,
#                      labels = nodes_labels 
#                    )
    
# --- Compute information theoretic measures for partitions of External Degree Models ---
    
#cpdef double log2( double x ):
#    return np.log( x ) / np.log( 2. )
                      
#cpdef double xlog2x( double x ):
#    assert x >= 0. and x <= 1. , 'ERROR @ xlog2x : x >= 0. or x <= 1. failed.'
#    if x == 0. or x == 1.:
#        return 0.
#    return x * log2( x )

#def check_partition( partition ):
#    def flatten_partition( partition ):
#        return [ item for sublist in partition for item in sublist ]
#    fpartition  = flatten_partition( partition )
#    sfpartition = set( fpartition )
#    assert len( fpartition ) == len( sfpartition ) , 'ERROR : len( fpartition ) == len( sfpartition ) failed.'
#    return sfpartition
    
#def entropy( partition ):
#    cdef double suma
#    cdef double H
#    cdef int n
#    cdef double x
#    
#    suma = 0.
#    for part in partition:
#        suma += float( len( part ) )
        
#    H = 0.
#    for part in partition:
#        n = len( part )
#        assert len( set( part ) ) == n , 'ERROR : assert len( set( part ) ) == n failed.'
#        x = float( len( part ) ) / suma
#        H -= xlog2x( x )
#        
#    return H
    
#def join_partition( partition1 , partition2 ):
#    sfpartition1 = check_partition( partition1 )
#    sfpartition2 = check_partition( partition2 )          
#    assert sfpartition1 == sfpartition2 , 'ERROR : sfpartition1 == sfpartition2 failed'
#    partition = []
#    for part1 , part2 in itertools.product( partition1 , partition2 ):
#        s1 = set( part1 )
#        s2 = set( part2 )        
#        assert len( s1 ) == len( part1 ) , 'ERROR : len( s1 ) == len( part1 ) failed.'
#        assert len( s2 ) == len( part2 ) , 'ERROR : len( s2 ) == len( part2 ) failed.'
#        part = list( s1 & s2 )
#        partition.append( part )
#    return partition

#def join_entropy( partition1 , partition2 ):
#    jp = join_partition( partition1 , partition2 )
#    return entropy( jp )
    
#def mutual_information( partition1 , partition2 ):
#    return entropy( partition1 ) + entropy( partition2 ) - join_entropy( partition1 , partition2 )
    
#def conditional_entropy( partition1 , partition2 ):
#    return join_entropy( partition1 , partition2 ) - entropy( partition2 )
    
#def normalized_mutual_information( partition1 , partition2 , method = lambda x,y: max( x , y ) ):
#    MI = mutual_information( partition1 , partition2 )
#    SI1 = mutual_information( partition1 , partition1 )
#    SI2 = mutual_information( partition2 , partition2 )
#    denominator = method( SI1 , SI2 )
#    if denominator != 0.:
#        return MI / denominator
#    return None

#def variation_of_information( partition1 , partition2 ):
#    return 2. * join_entropy( partition1 , partition2 ) - entropy( partition1 ) - entropy( partition2 )

#def adjusted_mutual_information( partition1 , partition2 ): #, method = lambda x,y: max( x , y ) ):
#    enumerate_elements = utils.Enumerate()
#    for part in partition1:
#        for element in part:
#            enumerate_elements[ element ]
#    len1 = enumerate_elements.len()
#    for part in partition2:
#        for element in part:
#            enumerate_elements[ element ]
#    len2 = enumerate_elements.len()
#    assert len1 == len2 , 'ERROR @ adjusted_mutual_information : assert len1 == len2'
#    array_partition_1 = [ 0 ] * enumerate_elements.len()
#    array_partition_2 = [ 0 ] * enumerate_elements.len()    
#    for label , part in enumerate( partition1 ):
#        for element in part:
#            array_partition_1[ enumerate_elements[ element ] ] = label
#    for label , part in enumerate( partition2 ):
#        for element in part:
#            array_partition_2[ enumerate_elements[ element ] ] = label
#    return adjusted_mutual_info_score( array_partition_1 , array_partition_2 )
    
#def similarity_order_parameter_sampler( partition_list_1 , partition_list_2 = None , method = lambda x,y: max( x , y ) ):
#    """
#    THEROY
#    ------
#    # TODO
#    """
#    if partition_list_2 is None:
#        for partition1 , partition2 in itertools.combinations( partition_list_1 , 2 ):
#            yield normalized_mutual_information( partition1 , partition2 , method = method )
#    else:
#        for partition1 in partition_list_1:
#            for partition2 in partition_list_2:
#                yield normalized_mutual_information( partition1 , partition2 , method = method )

#def adjusted_similarity_order_parameter_sampler( partition_list_1 , partition_list_2 = None ):
#    """
#    THEROY
#    ------
#    # TODO
#    """
#    if partition_list_2 is None:
#        for partition1 , partition2 in itertools.combinations( partition_list_1 , 2 ):
#            yield adjusted_mutual_information( partition1 , partition2 )
#    else:
#        for partition1 in partition_list_1:
#            for partition2 in partition_list_2:
#                yield adjusted_mutual_information( partition1 , partition2 )
