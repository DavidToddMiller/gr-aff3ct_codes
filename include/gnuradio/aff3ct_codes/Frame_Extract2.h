/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_AFF3CT_CODES_FRAME_EXTRACT2_H
#define INCLUDED_AFF3CT_CODES_FRAME_EXTRACT2_H

#include <gnuradio/aff3ct_codes/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace aff3ct_codes {

/*!
 * \brief <+description of block+>
 * \ingroup aff3ct_codes
 *
 */
class AFF3CT_CODES_API Frame_Extract2 : virtual public gr::block
{
public:
    typedef std::shared_ptr<Frame_Extract2> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of aff3ct_codes::Frame_Extract2.
     *
     * To avoid accidental use of raw pointers, aff3ct_codes::Frame_Extract2's
     * constructor is in a private implementation
     * class. aff3ct_codes::Frame_Extract2::make is the public interface for
     * creating new instances.
     */
    static sptr make(int frame_length, int buffer_length, int ASM_length);
};

} // namespace aff3ct_codes
} // namespace gr

#endif /* INCLUDED_AFF3CT_CODES_FRAME_EXTRACT2_H */
