import numpy as np

from sklearn.metrics.cluster import contingency_matrix
from sklearn.metrics.cluster import entropy as sklrn_entropy
from sklearn.metrics.cluster import mutual_info_score
from sklearn.metrics.cluster import expected_mutual_information
from sklearn.metrics.cluster import adjusted_mutual_info_score

cimport numpy as np
#cimport cython
#from libcpp cimport bool
#from libcpp.set cimport set as cset
#from libcpp.map cimport map as cmap
#from libcpp.pair cimport pair as cpair
from cython.operator cimport dereference as deref, preincrement as inc
#from edm cimport *
from partition cimport *

def PyNij( PyPartition pp1 , PyPartition pp2 ):
    cdef t_Matrix_int Nij

    assert pp1.num_elements() == pp2.num_elements() , 'ERROR @ PyNij : assert pp1.num_elements() == pp2.num_elements()'
    compute_Nij( deref( pp1._c_partition ) , deref( pp2._c_partition ) , Nij )

    np_Nij = np.zeros( ( Nij.num_rows() , Nij.num_cols() ) , dtype = np.int32 )
    for i in xrange( Nij.num_rows() ):
        for j in xrange( Nij.num_cols() ):
            np_Nij[ i , j ] = Nij.get_value( i , j )
    
    return np_Nij

def PyComputeInfoMeasures( PyPartition pp1 , PyPartition pp2 ):
    cdef t_Matrix_int Nij
    cdef double Hi = 0.
    cdef double Hj = 0.
    cdef double Hij = 0.
    cdef double MI = 0.
    cdef double NMIa = 0.
    cdef double NMIg = 0.
    cdef double NMIm = 0.
    cdef double NMImi = 0.
    cdef double EMI = 0.
    cdef double AMI = 0.

    assert pp1.num_elements() == pp2.num_elements() , 'ERROR @ PyComputeInfoMeasures : assert pp1.num_elements() == pp2.num_elements()'

    compute_Nij( deref( pp1._c_partition ) , deref( pp2._c_partition ) , Nij )
    compute_all_info_measures( Nij , Hi , Hj , Hij , MI , NMIa , NMIg , NMIm , NMImi , EMI , AMI )

    return { 'Hi' : Hi , 'Hj' : Hj , 'Hij' : Hij , 'MI' : MI , 'NMIa' : NMIa , 'NMIg' : NMIg , 'NMIm' : NMIm , 'NMImi' : NMImi , 'EMI' : EMI , 'AMI' : AMI }

def PyMI( PyPartition pp1 , PyPartition pp2 ):
    cdef t_Matrix_int Nij

    assert pp1.num_elements() == pp2.num_elements() , 'ERROR @ PyComputeInfoMeasures : assert pp1.num_elements() == pp2.num_elements()'
    compute_Nij( deref( pp1._c_partition ) , deref( pp2._c_partition ) , Nij )
    return MI( Nij )

def PyNMIa( PyPartition pp1 , PyPartition pp2 ):
    cdef t_Matrix_int Nij

    assert pp1.num_elements() == pp2.num_elements() , 'ERROR @ PyComputeInfoMeasures.PyNMIa : assert pp1.num_elements() == pp2.num_elements()'
    compute_Nij( deref( pp1._c_partition ) , deref( pp2._c_partition ) , Nij )
    return NMIa( Nij )

def PyNMIg( PyPartition pp1 , PyPartition pp2 ):
    cdef t_Matrix_int Nij

    assert pp1.num_elements() == pp2.num_elements() , 'ERROR @ PyComputeInfoMeasures.PyNMIg : assert pp1.num_elements() == pp2.num_elements()'
    compute_Nij( deref( pp1._c_partition ) , deref( pp2._c_partition ) , Nij )
    return NMIg( Nij )

def PyNMIm( PyPartition pp1 , PyPartition pp2 ):
    cdef t_Matrix_int Nij

    assert pp1.num_elements() == pp2.num_elements() , 'ERROR @ PyComputeInfoMeasures.PyNMIm : assert pp1.num_elements() == pp2.num_elements()'
    compute_Nij( deref( pp1._c_partition ) , deref( pp2._c_partition ) , Nij )
    return NMIm( Nij )

def PyNMImi( PyPartition pp1 , PyPartition pp2 ):
    cdef t_Matrix_int Nij

    assert pp1.num_elements() == pp2.num_elements() , 'ERROR @ PyComputeInfoMeasures.PyNMImi : assert pp1.num_elements() == pp2.num_elements()'
    compute_Nij( deref( pp1._c_partition ) , deref( pp2._c_partition ) , Nij )
    return NMImi( Nij )

def PyNMI( PyPartition pp1 , PyPartition pp2 ):
    cdef t_Matrix_int Nij

    assert pp1.num_elements() == pp2.num_elements() , 'ERROR @ PyComputeInfoMeasures.PyNMI : assert pp1.num_elements() == pp2.num_elements()'
    compute_Nij( deref( pp1._c_partition ) , deref( pp2._c_partition ) , Nij )
    return NMI( Nij )

def PyEMI( PyPartition pp1 , PyPartition pp2 ):
    cdef t_Matrix_int Nij

    assert pp1.num_elements() == pp2.num_elements() , 'ERROR @ PyComputeInfoMeasures.PyEMI : assert pp1.num_elements() == pp2.num_elements()'
    compute_Nij( deref( pp1._c_partition ) , deref( pp2._c_partition ) , Nij )
    return EMI( Nij )

def PyAMI( PyPartition pp1 , PyPartition pp2 ):
    cdef t_Matrix_int Nij

    assert pp1.num_elements() == pp2.num_elements() , 'ERROR @ PyComputeInfoMeasures.PyAMI : assert pp1.num_elements() == pp2.num_elements()'
    compute_Nij( deref( pp1._c_partition ) , deref( pp2._c_partition ) , Nij )
    return AMI( Nij )

def label1_and_label2_from_pp1_and_pp2( PyPartition pp1 , PyPartition pp2 ):

    assert pp1.num_elements() == pp2.num_elements() , 'ERROR @ label1_and_label2_from_pp1_and_pp2 : assert pp1.num_elements() == pp2.num_elements()'
    labels1 = [ 0 ] * pp1.num_elements()
    labels2 = [ 0 ] * pp2.num_elements()
    for i , e in enumerate( pp1.it_elements() ):
        l1 = pp1.label_of_element( e )
        l2 = pp2.label_of_element( e )
        labels1[ i ] = l1
        labels2[ i ] = l2
    return labels1 , labels2

def Nij_from_scikit( PyPartition pp1 , PyPartition pp2 ):

    labels1 , labels2 = label1_and_label2_from_pp1_and_pp2( pp1 , pp2 )
    return contingency_matrix( labels1 , labels2 , sparse = False )

def Entropies_from_scikit( PyPartition pp1 , PyPartition pp2 ):

    labels1 , labels2 = label1_and_label2_from_pp1_and_pp2( pp1 , pp2 )
    Hi , Hj = sklrn_entropy( labels1 ) , sklrn_entropy( labels2 )
    MI = MI_from_scikit( pp1 , pp2 )
    Hij = Hi + Hj - MI
    return Hi , Hj , Hij

def MI_from_scikit( PyPartition pp1 , PyPartition pp2 ):

    labels1 , labels2 = label1_and_label2_from_pp1_and_pp2( pp1 , pp2 )
    return mutual_info_score( labels1 , labels2 )

def EMI_from_scikit( PyPartition pp1 , PyPartition pp2 ):

    labels1 , labels2 = label1_and_label2_from_pp1_and_pp2( pp1 , pp2 )
    return expected_mutual_information( labels1 , labels2 )

def AMI_from_scikit( PyPartition pp1 , PyPartition pp2 ):

    labels1 , labels2 = label1_and_label2_from_pp1_and_pp2( pp1 , pp2 )
    return adjusted_mutual_info_score( labels1 , labels2 )
