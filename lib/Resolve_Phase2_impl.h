/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_AFF3CT_CODES_RESOLVE_PHASE2_IMPL_H
#define INCLUDED_AFF3CT_CODES_RESOLVE_PHASE2_IMPL_H

#include <gnuradio/aff3ct_codes/Resolve_Phase2.h>

namespace gr {
namespace aff3ct_codes {

class Resolve_Phase2_impl : public Resolve_Phase2
{
private:
    // Nothing to declare in this block.
    int s[64];
    int s90[64];
    int d_waveform;
    int d_frame_length;
    int d_buffer_length;
    int d_threshold;

public:
    Resolve_Phase2_impl(int waveform, int frame_length, int buffer_length, int threshold);
    ~Resolve_Phase2_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace aff3ct_codes
} // namespace gr

#endif /* INCLUDED_AFF3CT_CODES_RESOLVE_PHASE2_IMPL_H */
