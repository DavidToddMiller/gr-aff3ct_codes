/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_AFF3CT_CODES_TAG_FRAMEASM64_FF_IMPL_H
#define INCLUDED_AFF3CT_CODES_TAG_FRAMEASM64_FF_IMPL_H

#include <gnuradio/aff3ct_codes/Tag_FrameASM64_ff.h>

namespace gr {
namespace aff3ct_codes {

class Tag_FrameASM64_ff_impl : public Tag_FrameASM64_ff
{
private:
    // Nothing to declare in this block.
       unsigned long long d_access_code; // access code to locate start of Frame 
                                      //   access code is left justified in the word
       unsigned long long d_access_code90; // access code to locate start of Frame
                                      //   access code is left justified in the word

       unsigned long long d_data_reg;    // used to look for access_code
       unsigned int d_data_reg_bits = 0; // used to make sure we've seen the whole code
       unsigned long long d_mask;        // masks access_code bits (top N bits are set where
                                      //   N is the number of bits in the access code)
       unsigned long long d_mask90;        // masks access_code bits (top N bits are set where
                                      //   N is the number of bits in the access code)

       unsigned int d_waveform;    
       unsigned int d_len;               // the length of the ASM
       uint64_t d_threshold;         // how many bits may be wrong in sync ASM

       const std::string s64;
       const std::string s64rotate;


       pmt::pmt_t d_key, d_me; // d_key is the tag name, d_me is the block name + unique ID
       gr::thread::mutex d_mutex_access_code;
       gr::thread::mutex d_mutex_access_code90;
       gr::thread::mutex d_mutex_access_code48;
       gr::thread::mutex d_mutex_access_code4890;

       const std::string* access_code;  
       const std::string* access_code90; 


public:
    Tag_FrameASM64_ff_impl(int waveform, int ASM_length, int threshold);
    ~Tag_FrameASM64_ff_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);

       bool set_access_code(const std::string& access_code);
       bool set_access_code90(const std::string& access_code90);
};

} // namespace aff3ct_codes
} // namespace gr

#endif /* INCLUDED_AFF3CT_CODES_TAG_FRAMEASM64_FF_IMPL_H */
