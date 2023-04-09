/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_AFF3CT_CODES_CCSDS_PSEUDO_RANDOMIZER_IMPL_H
#define INCLUDED_AFF3CT_CODES_CCSDS_PSEUDO_RANDOMIZER_IMPL_H

#include <gnuradio/aff3ct_codes/CCSDS_Pseudo_Randomizer.h>

#include <gnuradio/digital/lfsr.h>

namespace gr {
namespace aff3ct_codes {

class CCSDS_Pseudo_Randomizer_impl : public CCSDS_Pseudo_Randomizer
{
private:
    // Nothing to declare in this block.
    digital::lfsr d_lfsr;
    int64_t d_count;  //!< Reset the LFSR after this many bytes (not bits)
    uint64_t d_mask;  
    uint8_t d_len;
    uint64_t d_seed;
    uint8_t d_bits_per_byte;



public:
    CCSDS_Pseudo_Randomizer_impl(int length, int count, int bits_per_byte);
    ~CCSDS_Pseudo_Randomizer_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace aff3ct_codes
} // namespace gr

#endif /* INCLUDED_AFF3CT_CODES_CCSDS_PSEUDO_RANDOMIZER_IMPL_H */
