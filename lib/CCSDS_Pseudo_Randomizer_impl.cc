/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "CCSDS_Pseudo_Randomizer_impl.h"
#include <gnuradio/io_signature.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>

namespace gr {
namespace aff3ct_codes {

using input_type = float;
using output_type = float;
CCSDS_Pseudo_Randomizer::sptr
CCSDS_Pseudo_Randomizer::make(int length, int count, int bits_per_byte)
{
    return gnuradio::make_block_sptr<CCSDS_Pseudo_Randomizer_impl>(
        length, count, bits_per_byte);
}


/*
 * The private constructor
 */
CCSDS_Pseudo_Randomizer_impl::CCSDS_Pseudo_Randomizer_impl(int length,
                                                           int count,
                                                           int bits_per_byte)
    : gr::block("CCSDS_Pseudo_Randomizer",
                gr::io_signature::make(
                    1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                gr::io_signature::make(
                    1 /* min outputs */, 1 /*max outputs */, sizeof(output_type))),
      d_lfsr(0xA9, 0xFF, 7),
      d_count(2048),    //int64_t // reset every 2048 bits rather than use tags
      //d_bytes(0),     // uint64_t  !< Count the processed bytes in WORK function but starts at
      d_mask(0xA9),     // Polynomial mask for LFSR and specific 0xA9 for CCSDS - uint64_t mask
      d_len(7),       //  uint8_t  Shift register length
      d_seed(0xFF),  //  int64_t and vakue for CCSDS = 0xFF, Initial shift register contents 
      d_bits_per_byte(1)   // int64_t 
{
}

/*
 * Our virtual destructor.
 */
CCSDS_Pseudo_Randomizer_impl::~CCSDS_Pseudo_Randomizer_impl() {}

void CCSDS_Pseudo_Randomizer_impl::forecast(int noutput_items,
                                            gr_vector_int& ninput_items_required)
{
 ninput_items_required[0] = noutput_items;
}

int CCSDS_Pseudo_Randomizer_impl::general_work(int noutput_items,
                                               gr_vector_int& ninput_items,
                                               gr_vector_const_void_star& input_items,
                                               gr_vector_void_star& output_items)
{
    // const unsigned char* in = (const unsigned char*)input_items[0];
   //  unsigned char* out = (unsigned char*)output_items[0];

    const float* in = (const float*)input_items[0];  //Each float item takes up 4 bytes in memory
    float* out = (float*)output_items[0]; // need to make output a "char/byte"

    // int reset_index = _get_next_reset_index(noutput_items); hardwire to i = 2048
    d_lfsr.reset();

    int8_t cc = 1;
    int8_t* AA;
    AA = &cc;
    int8_t AA_value;
    float_t AA_convert;

    for (int i = 0; i < 2048; i++)
    {   // hardwire to 2048;
        // Reset should occur at/before the item associated with the tag if tags used.
         //starts at reset_index of 2048  with d_bytes at 0 at start

        const float_t arrin = in[i];
//        arrin = in[i];       
        const float_t *arr_ptr;
        arr_ptr = &arrin;

        volk_32f_binary_slicer_8i(AA, arr_ptr, 1); 

        unsigned char scramble_byte = 0x00;
        //for (int k = 0; k < d_bits_per_byte; k++) {
        scramble_byte ^= (d_lfsr.next_bit() << 0);
        //}
         
        AA_value = *AA ^ scramble_byte;

        AA_convert = (float_t)AA_value;
        AA_convert = AA_convert * 2 - 1;
        
        out[i] = AA_convert * abs(arrin);  // * AA_convert;
        //out[i] = in[i] ^ scramble_byte;
    }



      //const float* in = (const float*)input_items[0];  //Each float item takes up 4 bytes in memory
      //float* out = (float*)output_items[0]; // need to make output a "char/byte"

    //for (int k = 0; k < noutput_items; k++) {
        //out[k] = in[k];
    //}

    consume_each(2048); // (noutput_items);

    // Tell runtime system how many output items we produced.
    return 2048;   // noutput_items;
}

} /* namespace aff3ct_codes */
} /* namespace gr */
