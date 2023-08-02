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
/* BINDTOOL_HEADER_FILE(dvbs2_ldpc_decoder.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(261205c09e31b0ed474ddb0257095f2c)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/aff3ct_codes/dvbs2_ldpc_decoder.h>
// pydoc.h is automatically generated in the build directory
#include <dvbs2_ldpc_decoder_pydoc.h>

void bind_dvbs2_ldpc_decoder(py::module& m)
{

    using dvbs2_ldpc_decoder    = gr::aff3ct_codes::dvbs2_ldpc_decoder;


    py::class_<dvbs2_ldpc_decoder, gr::block, gr::basic_block,
        std::shared_ptr<dvbs2_ldpc_decoder>>(m, "dvbs2_ldpc_decoder", D(dvbs2_ldpc_decoder))

        .def(py::init(&dvbs2_ldpc_decoder::make),
           D(dvbs2_ldpc_decoder,make)
        )
        



        ;




}








