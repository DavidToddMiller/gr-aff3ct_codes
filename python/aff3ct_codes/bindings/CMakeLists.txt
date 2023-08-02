# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT aff3ct_codes_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT aff3ct_codes_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND aff3ct_codes_python_files
    ccsds_ldpc_decoder_python.cc
    Tag_FrameASM64_ff_python.cc
    Frame_Extract2_python.cc
    Resolve_Phase2_python.cc
    CCSDS_Pseudo_Randomizer_python.cc
    Frame_ExtractOQPSK_python.cc
    dvbs2_ldpc_decoder_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(aff3ct_codes
   ../../..
   gr::aff3ct_codes
   "${aff3ct_codes_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET aff3ct_codes_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:aff3ct_codes_python>
    ${PROJECT_BINARY_DIR}/test_modules/gnuradio/aff3ct_codes/
)

install(TARGETS aff3ct_codes_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/aff3ct_codes COMPONENT pythonapi)
