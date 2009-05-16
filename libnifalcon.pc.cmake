libdir=@CMAKE_INSTALL_PREFIX@/lib
includedir=@CMAKE_INSTALL_PREFIX@/include

Name: @PC_SHORT_NAME@
Description: @PC_LONG_NAME@: @PC_DESCRIPTION@
Requires: @PC_REQUIRES@
Version: @PC_VERSION@
Libs: -L${libdir} @PC_LINK_FLAGS@
Cflags: -I${includedir} @PC_COMPILE_FLAGS@
