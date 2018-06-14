from collections import defaultdict

class Enumerate:
    """
    >>> enum = Enumerate()
    >>> enum.insert( 'a' )
    0
    >>> enum[ 'a' ]
    0
    >>>
    >>> enum
    Enumerate{'a'}
    >>>
    >>> enum.insert( '5' )
    1
    >>> enum[ '5' ]
    1
    >>>
    >>> enum
    Enumerate{'a','5'}
    >>>
    >>> enum.insert( 100 )
    2
    >>> enum[100]
    2
    >>>
    >>> enum
    Enumerate{'a','5',100}
    >>>
    >>> enum.insert( 100 )
    2
    >>> enum[100]
    2
    >>>
    >>> enum['5']
    1
    >>>
    >>> len( enum )
    3
    >>>
    >>> for l in enum:
    ...     print l
    a
    5
    100
    """
    def __init__(self,_enumerate=None):
        if _enumerate is None:
            self._max_i=-1
            self._l2i=defaultdict(self._next_i)
            self._i2l=[]
        else:
            assert isinstance(_enumerate,Enumerate),'ERROR: assert isinstance(_enumerate,Enumerate)'
            self._max_i=_enumerate.get_max_i()
            self._l2i=_enumerate.copy_l2i()
            self._i2l=_enumerate.copy_i2l()
    def len(self):
        return self._max_i+1
    def __len__(self):
        return self.len()
    def _next_i(self):
        self._i2l.append(self._last_l)
        self._max_i+=1
        return self._max_i
    def __getitem__(self,l):
        return self.l2i(l)
    def insert(self,l):
        self._last_l=l
        return self._l2i[l]
    def __contains__(self,l):
        return l in self._l2i
    def __iter__(self):
        #for l in self._l2i:
        for l in self._i2l:
            yield l
    def i2l(self,i):
        assert i>=0 and i<= self._max_i
        return self._i2l[i]
    def l2i(self,l):
        try:
            assert l in self._l2i
        except AssertionError as error:
            err_message  = "ERROR @ Enumerate.l2i : assert l in self._l2i" 
            err_message += "\n l = " + str( l ) 
            err_message += "\n self = " + str( self )
            error.args += ( err_message , ) # wrap it up in new tuple
            raise
        return self._l2i[l]
    def list_l(self):
        return self._l2i.keys()
    def copy(self):
        return Enumerate(_enumerate=self)
    def get_max_i(self):
        return self._max_i
    def copy_l2i(self):
        return self._l2i.copy()
    def copy_i2l(self):
        return list(self._i2l)
    def __repr__( self ):
        s = 'Enumerate{'
        sep = ''
        for i in xrange( len( self ) ):
            l = self.i2l( i )
            if isinstance( l , str ):
                ll = "'" + l + "'"
            else:
                ll = str( l )
            #s += sep + '(' + ll + ',' + str( self[ l ] ) + ')'
            s += sep + ll
            sep = ','
        return s + '}'

def rel_diff( a , b , tol = 0. ):
    diff = a - b
    norm = abs( a ) + abs( b ) + tol
    if norm == 0.:
        return 0.
    return 2. * diff / norm

class Bisection_function_return:
    def __init__( self , value ):
        self.value = value

def bisection_algorithm( func , a , b , tol , max_iter ):
    assert a < b
    assert tol > 0.
    assert max_iter > 0
    x_a = a
    x_b = b
    x_m = .5 * ( x_a + x_b )
    f_a = func( x_a )
    f_m = func( x_m )
    f_b = func( x_b )
    min_value = min( [ f_a.value , f_m.value , f_b.value ] )
    max_value = max( [ f_a.value , f_m.value , f_b.value ] )
    num_iter = 0
    while abs( x_b - x_a ) > tol and rel_diff_tol( min_value , max_value , tol ) > tol and num_iter < max_iter:
        num_iter += 1
        x_l = .5 * ( x_a + x_m )
        x_r = .5 * ( x_m + x_b )
        f_l = func( x_l )
        f_r = func( x_r )
        min_value = min( [ f_a.value , f_l.value , f_m.value , f_r.value , f_b.value ] )
        max_value = max( [ f_a.value , f_l.value , f_m.value , f_r.value , f_b.value ] )
        if   min_value == f_a.value or min_value == f_l.value:
            x_b = x_m
            x_m = x_l
            f_b = f_m
            f_m = f_l
        elif min_value == f_m.value:
            x_a = x_l
            x_b = x_r
            f_a = f_l
            f_b = f_r
        elif min_value == f_r.value or min_value == f_b.value:
            x_a = x_m
            x_m = x_r
            f_a = f_m
            f_m = f_r
        else:
            assert False , 'ERROR @ utils.bisection_algorithm : min_value != f_a, f_l, f_m, f_r, f_b'
    converged = num_iter < max_iter and abs( x_b - x_a ) < tol
    return x_m , f_m , num_iter , converged

if __name__ == "__main__":
    import doctest
    doctest.testmod()
