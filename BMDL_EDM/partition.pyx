# distutils: language = c++

# DEPRECATED
# # # distutils: sources = ExternalDegreeModel/lib/cpp_graph.cpp ExternalDegreeModel/lib/cpp_edm.cpp

# === Imports ===

import networkx as nx
import numpy as np
import itertools
from sklearn.metrics.cluster import adjusted_mutual_info_score
import utils

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

cdef class PyPartition:
    """
    Implements the PythonWrapper of the Partition C++ class.
    """

    # === Data-members ===
    
    # @ edm.pxd
    
    # === Function-members ===    
    
    def __cinit__( self , list_of_lists_partition ):

        cdef int i
        cdef int l

        elements = []
        for part in list_of_lists_partition:
            elements += part
        self._size = len( elements )
        assert self._size == len( set( elements ) ) , 'ERROR @ PyPartition.__cinint__ : There are repaeated elements in the "list_of_lists_partition".'

        self._e_enum = utils.Enumerate()       
        for i , e in enumerate( sorted( elements ) ):
            i = self._e_enum.insert( e )

        self._c_partition = new Partition( self._size )
        for l , part in enumerate( list_of_lists_partition ):
            for e in part:
                i = self._e_enum.l2i( e )
                self._c_partition.change_label_of_element( i , l )
            
    def __dealloc__( self ):
    
        del self._c_partition

    def copy( self ):

        copy_partition = PyPartition( self.to_list_of_lists() )

        return copy_partition

    def __repr__( self ):
        s = 'Partition{'
        sep = " "
        for l in self.it_labels():
            s += sep + str( l ) + ":"
            for e in self.it_elements( l ):
                s += " " + str( e ) + "." + str( self._e_enum.l2i( e ) )
            sep = " | "
        return s + " }"
      
    def to_list_of_lists( self ):

        return [ [ e for e in self.it_elements( l ) ] for l in self.it_labels() ]

    def to_dict( self ):

        return { l : [ e for e in self.it_elements( l ) ] for l in self.it_labels() }

    def label_of_element( self , e ):
        cdef int i

        i = self._e_enum.l2i( e )
        return self._c_partition.label_of_element( i )
        
    def set_element_label( self , e , l ):
        cdef int i       
 
        assert l >= 0 and l < self._size , 'ERROR @ PyPartition.set_element_label : label l out of range.'
        i = self._e_enum.l2i( e )
        self._c_partition.change_label_of_element( i , l ) 

    def is_label_active( self , l ):

        assert l >= 0 and l < self._size , 'ERROR @ PyPartition.set_element_label : label l out of range.'
        return self._c_partition.is_label_active( l )

    def num_elements( self ):
   
        return self._size

    def num_active_labels( self ):
    
        return self._c_edm.num_active_labels()        

    def element_idx( self , e ):

        return self._e_enum.l2i( e )
        
    def it_labels( self ):
        cdef Partition.t_label_iterator l_it = self._c_partition.label_it()
        cdef int l

        while l_it.keepgoing():
            l = l_it.deref()
            l_it.inc()
            yield l

    def it_elements( self , l = None ):
        cdef Partition.t_element_iterator i_it
        cdef int i
        
        if l is None:
            #for i in self._G.nodes():
            for e in self._e_enum:
                yield e
        else:
            assert l >= 0 and l < self._size , 'ERROR @ PyPartition.it_elements : label l out of range.'
            i_it = self._c_partition.element_it( l )
            while i_it.keepgoing():
                i = i_it.deref()
                i_it.inc()
                yield self._e_enum.i2l( i )

    def set_partition( self , list_of_lists_partition ):
        cdef int l
        cdef int i

        for l , part in enumerate( list_of_lists_partition ):
            for e in part:
                i = self._e_enum.l2i( e )
                self._c_partition.change_label_of_element( i , l )

    #cdef Partition * ptr_c_partition( self ):
    #    
    #    return self._c_partition 

# === Complements ===

