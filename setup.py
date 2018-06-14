from setuptools import setup, Extension
from Cython.Distutils import build_ext
import numpy as np

NAME = "BMDL_EDM"
VERSION = "0.3"
DESCR = "The BMDL-EDM and the B'MDL-EDM for community detection."
URL = "https://github.com/rayohauno/beta_RMDL_EDM"
#REQUIRES = [ 'numpy', 'cython' ]
with open( 'requirements.txt' , 'r' ) as fh:
    REQUIRES = fh.read().splitlines()

AUTHOR = "Juan I. Perotti"
EMAIL = "perotti at famaf dot unc dot edu dot ar"

LICENSE = "GPL3"

SRC_DIR = "BMDL_EDM"
PACKAGES = [ SRC_DIR ]

EXTENSIONS = []

#common_extra_compile_args = [ "-DNDEBUG" , "-O2" ]
#commom_extra_link_args = [ ]
common_extra_compile_args = [ "-std=c++11" , "-DNDEBUG" , "-O2" ]
commom_extra_link_args = [ "-std=c++11" ]

#ext_graph = Extension( SRC_DIR + ".graph",
#                       [ SRC_DIR + "/lib/cpp_graph.cpp" , 
#                         SRC_DIR + "/graph.pyx" ] ,
#                       #libraries = [] ,
#                       include_dirs = [ np.get_include() , 
#                                        SRC_DIR + '/lib' ] ,
#                       language = "c++" ,
#                       extra_compile_args = common_extra_compile_args ,
#                       extra_link_args = commom_extra_link_args                                 
#                     )
#EXTENSIONS += [ ext_graph ]

ext_random_eng   = Extension( SRC_DIR + ".random_eng",
#                       [ SRC_DIR + "/lib/cpp_graph.cpp" , 
#                       [ SRC_DIR + "/lib/cpp_static_graph.cpp" , 
                       [
#                         SRC_DIR + "/lib/cpp_edm.cpp" , 
                         SRC_DIR + "/lib/cpp_random.cpp" , 
                         SRC_DIR + "/random_eng.pyx" 
                       ] ,
                       #libraries = [] ,
                       include_dirs = [ np.get_include() , 
                                        SRC_DIR + '/lib' ] ,
                       language = "c++" ,
                       extra_compile_args = common_extra_compile_args ,
                       extra_link_args = commom_extra_link_args               
                     )
EXTENSIONS += [ ext_random_eng   ]

ext_edm   = Extension( SRC_DIR + ".edm",
#                       [ SRC_DIR + "/lib/cpp_graph.cpp" , 
#                       [ SRC_DIR + "/lib/cpp_static_graph.cpp" , 
                       [
#                         SRC_DIR + "/lib/cpp_edm.cpp" , 
                         SRC_DIR + "/edm.pyx" 
                       ] ,
                       #libraries = [] ,
                       include_dirs = [ np.get_include() , 
                                        SRC_DIR + '/lib' ] ,
                       language = "c++" ,
                       extra_compile_args = common_extra_compile_args ,
                       extra_link_args = commom_extra_link_args               
                     )
EXTENSIONS += [ ext_edm   ]
                     
ext_nhoa  = Extension( SRC_DIR + ".nhoa",
                       #[ SRC_DIR + "/lib/cpp_graph.cpp" , 
                       #  SRC_DIR + "/lib/cpp_edm.cpp" , 
                       [
#                         SRC_DIR + "/lib/cpp_nhoa.cpp" , 
                         SRC_DIR + "/nhoa.pyx" 
                       ] ,
                       #libraries = [] ,
                       include_dirs = [ np.get_include() , 
                                        SRC_DIR + '/lib' ] ,
                       language = "c++" ,
                       extra_compile_args = common_extra_compile_args ,
                       extra_link_args = commom_extra_link_args               
                     )        
EXTENSIONS += [ ext_nhoa  ]

ext_info_theory  = Extension( SRC_DIR + ".info_theory",
                       #[ SRC_DIR + "/lib/cpp_graph.cpp" , 
                       #  SRC_DIR + "/lib/cpp_edm.cpp" , 
                       [
#                         SRC_DIR + "/lib/cpp_nhoa.cpp" , 
                         SRC_DIR + "/info_theory.pyx" 
                       ] ,
                       #libraries = [] ,
                       include_dirs = [ np.get_include() , 
                                        SRC_DIR + '/lib' ] ,
                       language = "c++" ,
                       extra_compile_args = common_extra_compile_args ,
                       extra_link_args = commom_extra_link_args               
                     )        
EXTENSIONS += [ ext_info_theory  ]
                     
ext_partition  = Extension( SRC_DIR + ".partition",
                       #[ SRC_DIR + "/lib/cpp_graph.cpp" , 
                       #  SRC_DIR + "/lib/cpp_edm.cpp" , 
                       [
#                         SRC_DIR + "/lib/cpp_nhoa.cpp" , 
                         SRC_DIR + "/partition.pyx" 
                       ] ,
                       #libraries = [] ,
                       include_dirs = [ np.get_include() , 
                                        SRC_DIR + '/lib' ] ,
                       language = "c++" ,
                       extra_compile_args = common_extra_compile_args ,
                       extra_link_args = commom_extra_link_args               
                     )        
EXTENSIONS += [ ext_partition  ] 

ext_meta_edm  = Extension( SRC_DIR + ".meta_edm",
                       #[ SRC_DIR + "/lib/cpp_graph.cpp" , 
                       #  SRC_DIR + "/lib/cpp_edm.cpp" , 
                       [
#                         SRC_DIR + "/lib/cpp_nhoa.cpp" , 
                         SRC_DIR + "/meta_edm.pyx" 
                       ] ,
                       #libraries = [] ,
                       include_dirs = [ np.get_include() , 
                                        SRC_DIR + '/lib' ] ,
                       language = "c++" ,
                       extra_compile_args = common_extra_compile_args ,
                       extra_link_args = commom_extra_link_args               
                     )        
EXTENSIONS += [ ext_meta_edm  ]   
                   
ext_sbp_mod = Extension( SRC_DIR + ".sbp_mod",
                         #[ SRC_DIR + "/lib/cpp_graph.cpp" , 
                         #  SRC_DIR + "/lib/cpp_edm.cpp" , 
                         [
                           SRC_DIR + "/lib/cpp_random.cpp" , 
                           SRC_DIR + "/sbp_mod.pyx" 
                         ] ,
                         #libraries = [] ,
                         include_dirs = [ np.get_include() , 
                                          SRC_DIR + '/lib' ] ,
                         language = "c++" ,
                         extra_compile_args = common_extra_compile_args ,
                         extra_link_args = commom_extra_link_args               
                       )        
EXTENSIONS += [ ext_sbp_mod ]   

ext_sbp_rmdl = Extension( SRC_DIR + ".sbp_rmdl",
                         #[ SRC_DIR + "/lib/cpp_graph.cpp" , 
                         #  SRC_DIR + "/lib/cpp_edm.cpp" , 
                         [
                           SRC_DIR + "/lib/cpp_random.cpp" , 
                           SRC_DIR + "/sbp_rmdl.pyx" 
                         ] ,
                         #libraries = [] ,
                         include_dirs = [ np.get_include() , 
                                          SRC_DIR + '/lib' ] ,
                         language = "c++" ,
                         extra_compile_args = common_extra_compile_args ,
                         extra_link_args = commom_extra_link_args               
                       )        
EXTENSIONS += [ ext_sbp_rmdl ]   

#ext_mcsampling = Extension( SRC_DIR + ".mc_sampling",
#                            [ SRC_DIR + "/lib/cpp_graph.cpp" , 
#                              SRC_DIR + "/lib/cpp_edm.cpp" , 
#                              SRC_DIR + "/lib/cpp_utils.cpp" ,                               
#                              #SRC_DIR + "/lib/cpp_mc_sampling.cpp" , 
#                              SRC_DIR + "/mc_sampling.pyx" ] ,
#                            #libraries = [] ,
#                            include_dirs = [ np.get_include() , 
#                                             SRC_DIR + '/lib' ] ,
#                            language = "c++" ,
#                            extra_compile_args = common_extra_compile_args ,
#                            extra_link_args = commom_extra_link_args               
#                          )        
#EXTENSIONS += [ ext_mcsampling  ]

#ext_annealing  = Extension( SRC_DIR + ".annealing",
#                            [ SRC_DIR + "/lib/cpp_graph.cpp" , 
#                              SRC_DIR + "/lib/cpp_edm.cpp" , 
#                              SRC_DIR + "/lib/cpp_utils.cpp" ,                               
#                              SRC_DIR + "/lib/cpp_annealing.cpp" , 
#                              SRC_DIR + "/annealing.pyx" ] ,
#                            #libraries = [] ,
#                            include_dirs = [ np.get_include() , 
#                                             SRC_DIR + '/lib' ] ,
#                            language = "c++" ,
#                            extra_compile_args = common_extra_compile_args ,
#                            extra_link_args = commom_extra_link_args               
#                          )        
#EXTENSIONS += [ ext_annealing  ]
                     
#ext_hedm  = Extension( SRC_DIR + ".hedm",
#                       [ SRC_DIR + "/lib/cpp_graph.cpp" , 
#                         SRC_DIR + "/lib/cpp_edm.cpp" , 
#                         SRC_DIR + "/lib/cpp_nhoa.cpp" , 
#                         SRC_DIR + "/lib/cpp_hedm.cpp" , 
#                         SRC_DIR + "/hedm.pyx" ] ,
#                       #libraries = [] ,
#                       include_dirs = [ np.get_include() , 
#                                        SRC_DIR + '/lib' ] ,
#                       language = "c++" ,
#                       extra_compile_args = common_extra_compile_args ,
#                       extra_link_args = commom_extra_link_args               
#                     )                                            
#EXTENSIONS += [ ext_hedm  ]

if __name__ == "__main__":
    setup( install_requires = REQUIRES ,
           packages = PACKAGES ,
           zip_safe = False ,
           name = NAME ,
           version = VERSION ,
           description = DESCR ,
           author = AUTHOR ,
           author_email = EMAIL ,
           url = URL ,
           license = LICENSE ,
           cmdclass = { "build_ext" : build_ext } ,
           ext_modules = EXTENSIONS
         )       
       
