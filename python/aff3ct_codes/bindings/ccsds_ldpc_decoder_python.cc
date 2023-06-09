/*
 * Copyright 2023 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(ccsds_ldpc_decoder.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(31141f72e132ac6d001a4b7e94474455)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/aff3ct_codes/ccsds_ldpc_decoder.h>
// pydoc.h is automatically generated in the build directory
#include <ccsds_ldpc_decoder_pydoc.h>

void bind_ccsds_ldpc_decoder(py::module& m)
{

    using ccsds_ldpc_decoder    = gr::aff3ct_codes::ccsds_ldpc_decoder;


    py::class_<ccsds_ldpc_decoder, gr::block, gr::basic_block,
        std::shared_ptr<ccsds_ldpc_decoder>>(m, "ccsds_ldpc_decoder", D(ccsds_ldpc_decoder))

        .def(py::init(&ccsds_ldpc_decoder::make),
           D(ccsds_ldpc_decoder,make)
        )
        



        ;




}








