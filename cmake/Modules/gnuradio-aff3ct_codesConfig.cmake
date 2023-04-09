find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_AFF3CT_CODES gnuradio-aff3ct_codes)

FIND_PATH(
    GR_AFF3CT_CODES_INCLUDE_DIRS
    NAMES gnuradio/aff3ct_codes/api.h
    HINTS $ENV{AFF3CT_CODES_DIR}/include
        ${PC_AFF3CT_CODES_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_AFF3CT_CODES_LIBRARIES
    NAMES gnuradio-aff3ct_codes
    HINTS $ENV{AFF3CT_CODES_DIR}/lib
        ${PC_AFF3CT_CODES_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-aff3ct_codesTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_AFF3CT_CODES DEFAULT_MSG GR_AFF3CT_CODES_LIBRARIES GR_AFF3CT_CODES_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_AFF3CT_CODES_LIBRARIES GR_AFF3CT_CODES_INCLUDE_DIRS)
