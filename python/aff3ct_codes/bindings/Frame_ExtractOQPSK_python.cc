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
/* BINDTOOL_HEADER_FILE(Frame_ExtractOQPSK.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(b485cd92522c4f58dffd870ce73fc6b0)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/aff3ct_codes/Frame_ExtractOQPSK.h>
// pydoc.h is automatically generated in the build directory
#include <Frame_ExtractOQPSK_pydoc.h>

void bind_Frame_ExtractOQPSK(py::module& m)
{

    using Frame_ExtractOQPSK    = gr::aff3ct_codes::Frame_ExtractOQPSK;


    py::class_<Frame_ExtractOQPSK, gr::block, gr::basic_block,
        std::shared_ptr<Frame_ExtractOQPSK>>(m, "Frame_ExtractOQPSK", D(Frame_ExtractOQPSK))

        .def(py::init(&Frame_ExtractOQPSK::make),
           D(Frame_ExtractOQPSK,make)
        )
        



        ;




}







