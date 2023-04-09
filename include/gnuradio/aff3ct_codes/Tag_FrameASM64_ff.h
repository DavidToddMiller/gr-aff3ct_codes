/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_AFF3CT_CODES_TAG_FRAMEASM64_FF_H
#define INCLUDED_AFF3CT_CODES_TAG_FRAMEASM64_FF_H

#include <gnuradio/aff3ct_codes/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace aff3ct_codes {

/*!
 * \brief <+description of block+>
 * \ingroup aff3ct_codes
 *
 */
class AFF3CT_CODES_API Tag_FrameASM64_ff : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<Tag_FrameASM64_ff> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of aff3ct_codes::Tag_FrameASM64_ff.
     *
     * To avoid accidental use of raw pointers, aff3ct_codes::Tag_FrameASM64_ff's
     * constructor is in a private implementation
     * class. aff3ct_codes::Tag_FrameASM64_ff::make is the public interface for
     * creating new instances.
     */
    static sptr make(int waveform, int ASM_length, int threshold);
};

} // namespace aff3ct_codes
} // namespace gr

#endif /* INCLUDED_AFF3CT_CODES_TAG_FRAMEASM64_FF_H */
