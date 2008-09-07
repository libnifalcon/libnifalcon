# - try to find gmtl library and include files
# By Kyle Machulis for libnifalcon
# kyle@nonpolynomial.com
# Luckily, GMTL is nothing but headers, so this ain't that hard.
#  GMTL_INCLUDE_DIR, where to find gmtl/gmtl.h
#  GMTL_FOUND, If false, do not try to use GMTL

IF (WIN32)
  IF(CYGWIN)
    FIND_PATH( GMTL_INCLUDE_DIR gmtl/gmtl.h
      /usr/include
	  /usr/local/include
    )
  ELSE(CYGWIN)
    FIND_PATH( GMTL_INCLUDE_DIR gmtl/gmtl.h
      ${GMTL_ROOT_PATH}/include
    )
  ENDIF(CYGWIN)

ELSE (WIN32)
  FIND_PATH( GMTL_INCLUDE_DIR gmtl/gmtl.h
    /usr/include
    /usr/local/include
    ENV INCLUDE
    )
ENDIF (WIN32)

SET( GMTL_FOUND "NO" )
IF(GMTL_INCLUDE_DIR)
  message(STATUS "Found GMTL: ${GMTL_INCLUDE_DIR}")
  SET( GMTL_FOUND "YES" )
ENDIF(GMTL_INCLUDE_DIR)

MARK_AS_ADVANCED(
  GMTL_INCLUDE_DIR
)
