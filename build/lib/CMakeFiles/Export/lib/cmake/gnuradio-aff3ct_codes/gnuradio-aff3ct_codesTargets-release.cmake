#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gnuradio::gnuradio-aff3ct_codes" for configuration "Release"
set_property(TARGET gnuradio::gnuradio-aff3ct_codes APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(gnuradio::gnuradio-aff3ct_codes PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/x86_64-linux-gnu/libgnuradio-aff3ct_codes.so.1.0.0.0"
  IMPORTED_SONAME_RELEASE "libgnuradio-aff3ct_codes.so.1.0.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS gnuradio::gnuradio-aff3ct_codes )
list(APPEND _IMPORT_CHECK_FILES_FOR_gnuradio::gnuradio-aff3ct_codes "${_IMPORT_PREFIX}/lib/x86_64-linux-gnu/libgnuradio-aff3ct_codes.so.1.0.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
