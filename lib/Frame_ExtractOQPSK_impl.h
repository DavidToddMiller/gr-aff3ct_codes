/* -*- c++ -*- */
/*
 * Copyright 2023 D Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_AFF3CT_CODES_FRAME_EXTRACTOQPSK_IMPL_H
#define INCLUDED_AFF3CT_CODES_FRAME_EXTRACTOQPSK_IMPL_H

#include <gnuradio/aff3ct_codes/Frame_ExtractOQPSK.h>

namespace gr {
namespace aff3ct_codes {

class Frame_ExtractOQPSK_impl : public Frame_ExtractOQPSK
{
private:
    // Nothing to declare in this block.
    int d_frame_length;
    int d_buffer_length;
    int d_ASM_length;
    int n_dropped_times;


public:
    Frame_ExtractOQPSK_impl(int frame_length, int buffer_length, int ASM_length);
    ~Frame_ExtractOQPSK_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace aff3ct_codes
} // namespace gr

#endif /* INCLUDED_AFF3CT_CODES_FRAME_EXTRACTOQPSK_IMPL_H */
