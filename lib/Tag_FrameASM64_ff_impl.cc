/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "Tag_FrameASM64_ff_impl.h"
#include <gnuradio/io_signature.h>

#include <gnuradio/math.h>
#include <volk/volk.h>
#include <boost/format.hpp>
#include <cstdio>
#include <iostream>
#include <stdexcept>

namespace gr {
namespace aff3ct_codes {

using input_type = float;
using output_type = float;
Tag_FrameASM64_ff::sptr
Tag_FrameASM64_ff::make(int waveform, int ASM_length, int threshold)
{
    return gnuradio::make_block_sptr<Tag_FrameASM64_ff_impl>(
        waveform, ASM_length, threshold);
}


/*
 * The private constructor
 */
Tag_FrameASM64_ff_impl::Tag_FrameASM64_ff_impl(int waveform,
                                               int ASM_length,
                                               int threshold)
    : gr::sync_block("Tag_FrameASM64_ff",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type))),
      d_data_reg(0),
      d_mask(0),
      d_mask90(0),    
      d_waveform(waveform),
      d_len(ASM_length),
      d_threshold(threshold),   // type cast int to uint64_t

//  Sync Word in hex '0 3    4    7   7    6     C    7    2    7    2    8    9    5    B    0'
                 //'034776C7272895B0'
            s64{"0000001101000111011101101100011100100111001010001001010110110000"}, // 64 bit CCSDS ASM
      s64rotate{"1010100100100001000100110110000110110001101111101100000011011010"} // 64 bit CCSDS ASM
            //s32{"00 01 10 10 11 00 11 111111110000011101"}, // 32 bit CCSDS ASM
      //s32rotate{"10 00 11 11 01 10 01 010101011010000100"} // 32 bit CCSDS ASM with phase rotation 90 degrees

{
      const std::string& access_code = s64;  // 64 bit CCSDS ASM 
      set_access_code(access_code);
      const std::string& access_code90 = s64rotate; //64 bit ASM code for 90 degree rotation
      set_access_code90(access_code90);
 
    std::stringstream str;
    str << name() << unique_id();
    d_me = pmt::string_to_symbol(str.str());
    d_key = pmt::string_to_symbol("TagASM");
}

    bool Tag_FrameASM64_ff_impl::set_access_code(const std::string& access_code)
    {
        gr::thread::scoped_lock l(d_mutex_access_code);
        //d_len = 64; // # of bytes in string
        
        // set len bottom bits to 1.
        d_mask = ((~0ULL) >> (64-d_len));

        d_access_code = 0;
        for (unsigned i = 0; i < d_len; i++) 
        {
            d_access_code = (d_access_code << 1) | (access_code[i] & 1);
        }  

        GR_LOG_DEBUG(d_logger, boost::format("Access code: %llx") % d_access_code);
        GR_LOG_DEBUG(d_logger, boost::format("Mask: %llx") % d_mask);

        return true;
    }       

    bool Tag_FrameASM64_ff_impl::set_access_code90(const std::string& access_code90)
    {
        gr::thread::scoped_lock l(d_mutex_access_code90);
                
        // set len bottom bits to 1.
        d_mask90 = ((~0ULL) >> (64-d_len));
        d_access_code90 = 0;
        for (unsigned i = 0; i < d_len; i++) 
        {
            d_access_code90 = (d_access_code90 << 1) | (access_code90[i] & 1);
        }  
       GR_LOG_DEBUG(d_logger, boost::format("Access code90: %llx") % d_access_code90);
       GR_LOG_DEBUG(d_logger, boost::format("Mask90: %llx") % d_mask90);
       return true;
    }  

/*
 * Our virtual destructor.
 */
Tag_FrameASM64_ff_impl::~Tag_FrameASM64_ff_impl() {}


int Tag_FrameASM64_ff_impl::work(int noutput_items,
                                 gr_vector_const_void_star& input_items,
                                 gr_vector_void_star& output_items)
{

      const float* in = (const float*)input_items[0];
      float* out = (float*)output_items[0];

    uint64_t abs_out_sample_cnt = nitems_written(0);

   //use int d_waveform = 1 if BPSK and d_waveform = 2 if QPSK
    if(d_waveform == 1)  //BPSK
    {  
      for (int i = 0; i < noutput_items; i++) {
        out[i] = in[i];

        // compute hamming distance between desired access code and current data
        uint64_t wrong_bits = 0;
        uint64_t nwrong = d_threshold + 1;
        
        if (d_data_reg_bits < d_len) {
            d_data_reg_bits++;
        } else {
            wrong_bits = (d_data_reg ^ d_access_code) & d_mask;
            volk_64u_popcnt(&nwrong, wrong_bits);  // Volk kernal function - See Volk library
        }

        // shift in new data
        //d_data_reg = (d_data_reg << 1) | (gr::branchless_binary_slicer(in[i]) & 0x1);
        d_data_reg = (d_data_reg << 1) | (gr::branchless_binary_slicer(in[i]) & 0x1);
        if ( (nwrong <= d_threshold) | (nwrong >= (d_len-d_threshold)) ) {  // logic for BPSK phases
            d_logger->debug("writing tag at sample {:d}", abs_out_sample_cnt + i);
            //GR_LOG_DEBUG(d_logger, boost::format("Bits wrong in ASM %llu") % (nwrong));
            add_item_tag(0,                      // stream ID
                         abs_out_sample_cnt + i, // sample
                         d_key,                  // frame info
                         pmt::from_long(nwrong), // data (number wrong or nwrong90)
                         d_me                    // block src id
            );
        }
      }  // END of FOR loop for i
            // GR_LOG_DEBUG(d_logger, boost::format("Case 1 %llu") % (d_waveform));
    } // End of IF statement for BPSK 


    if(d_waveform == 2)  //QPSK
    {  
      for (int i = 0; i < noutput_items; i++) {
        out[i] = in[i];

        // compute hamming distance between desired access code and current data
        uint64_t wrong_bits = 0;
        uint64_t nwrong = d_threshold + 1;
        uint64_t wrong_bits90 = 0;
        uint64_t nwrong90 = d_threshold + 1;
        
        if (d_data_reg_bits < d_len) {
            d_data_reg_bits++;
        } else {
            wrong_bits = (d_data_reg ^ d_access_code) & d_mask;
            volk_64u_popcnt(&nwrong, wrong_bits);  // Volk kernal function - See Volk library
            wrong_bits90 = (d_data_reg ^ d_access_code90) & d_mask90;
            volk_64u_popcnt(&nwrong90, wrong_bits90);  // Volk kernal function - See Volk library
        }

        // shift in new data
        //d_data_reg = (d_data_reg << 1) | (gr::branchless_binary_slicer(in[i]) & 0x1);
        d_data_reg = (d_data_reg << 1) | (gr::branchless_binary_slicer(in[i]) & 0x1);
        if ( (nwrong <= d_threshold) | (nwrong >= (d_len-d_threshold)) | (nwrong90 <= d_threshold) | (nwrong90 >= (d_len-d_threshold))  ) {  // logic for QPSK phases
            d_logger->debug("writing tag at sample {:d}", abs_out_sample_cnt + i);
            //GR_LOG_DEBUG(d_logger, boost::format("Bits wrong in ASM %llu") % (nwrong));
            add_item_tag(0,                      // stream ID
                         abs_out_sample_cnt + i, // sample
                         d_key,                  // frame info
                         pmt::from_long(nwrong), // data (number wrong or nwrong90)
                         d_me                    // block src id
            );
        }

      }  // END of FOR loop for i

    } // END of IF statement for QPSK cases


    // Tell runtime system how many output items we produced.
    return noutput_items;
}


} /* namespace aff3ct_codes */
} /* namespace gr */
