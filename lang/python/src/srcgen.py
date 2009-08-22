
import os
from pyplusplus import module_builder

#Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t( [r"/Users/qdot/git-projects/libnifalcon/include/falcon/util/FalconDeviceDefault.h" ]
                                      , gccxml_path=r"" 
                                      , working_directory=r"/Users/qdot/git-projects/libnifalcon/include/falcon/core"
                                      , include_paths=['/Users/qdot/build/fiveten_libraries/usr_darwin_10.5_x86/include', '/Users/qdot/git-projects/libnifalcon/include/']
                                      , define_symbols=['LIBUSB'] )


#Well, don't you want to see what is going on?
#mb.print_declarations()

#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='falcondevice' )

#Writing code to file.
mb.write_module( 'FalconDevice_py.cpp' )
