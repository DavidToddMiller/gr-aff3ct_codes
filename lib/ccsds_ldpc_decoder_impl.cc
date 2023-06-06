/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "ccsds_ldpc_decoder_impl.h"
#include <gnuradio/io_signature.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>

#include <aff3ct.hpp>

namespace gr {
namespace aff3ct_codes {

using input_type = float;
using output_type = char;
ccsds_ldpc_decoder::sptr
ccsds_ldpc_decoder::make(int code_rate, bool data_rate, int decoder_iterations)
{
    return gnuradio::make_block_sptr<ccsds_ldpc_decoder_impl>(
        code_rate, data_rate, decoder_iterations);
}

/*
 * The private constructor
 */
ccsds_ldpc_decoder_impl::ccsds_ldpc_decoder_impl(int code_rate,
                                                 bool data_rate,
                                                 int decoder_iterations)
    : gr::block("ccsds_ldpc_decoder",
                gr::io_signature::make(
                    1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                gr::io_signature::make(
                    1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
{
    set_output_multiple(60000); 
       
      // Basic AFF3CT LDPC Parameters
      d_K = 1024;
      d_N = 2560;     // codeword size
      d_puncturing = 512;   // number of punctured bits
      d_n_ite = 40;        // number of decoder iterations

      for (int j = 0; j < d_K; ++j) {
        d_info_bits_pos.push_back(j);
      }

      // Construct LDPC H Matrix via alist
      //d_alist_file;  //set file name object for in-out
      d_alist_file.open("H_matrix_ccsds_ar4ja_r1_2_k1024.alist", std::ifstream::in);
      d_H = aff3ct::tools::AList::read(d_alist_file);


     // Modem-Decoder struct Object parameters
      d_m.d_modem = std::unique_ptr<aff3ct::module::Modem_BPSK<>>(new aff3ct::module::Modem_BPSK<>(d_N));
      d_m.d_decoder = std::unique_ptr<aff3ct::module::Decoder_LDPC_BP_flooding<>>(new aff3ct::module::Decoder_LDPC_BP_flooding<>(d_K, d_N, d_n_ite, d_H, d_info_bits_pos, d_update_rule));
   // "NEW": is used to give dynamic memory allocation when size of memory is variable
    // - must release memory or memory leak until program closed.

     // Buffer Struct values for GNURadio Block Input/Output Circular buffers interface
      d_b.d_sigma = std::vector<float>(1);
      d_b.d_noisy_symbols = std::vector<float>(d_N);
      d_b.d_LLRs = std::vector<float>(d_N);
      d_b.d_dec_bits = std::vector<int>(d_K);
      d_b.d_dec_bits_packed = std::vector<uint8_t>(d_K / 8);
}

/*
 * Our virtual destructor.
 */
ccsds_ldpc_decoder_impl::~ccsds_ldpc_decoder_impl() {}

void ccsds_ldpc_decoder_impl::forecast(int noutput_items,
                                       gr_vector_int& ninput_items_required)
{
 ninput_items_required[0] = noutput_items;
}

int ccsds_ldpc_decoder_impl::general_work(int noutput_items,
                                          gr_vector_int& ninput_items,
                                          gr_vector_const_void_star& input_items,
                                          gr_vector_void_star& output_items)
{
      const float* in = (const float*)input_items[0];  //Each float item takes up 4 bytes in memory
      char* out = (char*)output_items[0]; //char hard decision output after soft decision decoding
  
      uint64_t n_digested = 20*2048; // 20 frames for frame length 2048 
      uint64_t n_produced = 20*2048/16; // char output packed and associated rate 1/2
 
         // Initialise an array for b.noisy in/out interface with GNU Radio block circular buffers
      float arr[2560];
      float extra0 = 0.0;
      char arrout[128];

      std::fill(d_b.d_sigma.begin(), d_b.d_sigma.end(), 0.5);
              // "fill" from .begin to .end all set to 0.5 for each element/item

      for(int i=0; i<20; i++)  // each 2048 symbol frame in one WORK function
      {
           d_b.d_noisy_symbols.clear();  //container reset 0 size for each "i" increment 
            // Traverse the in/out into/outof array and d_b.d_noisy_symbols prior to 512 zeros
           for (int j = 0; j < 2560; j++)
           {
              if(j<2048)
              {
              arr[j] = *in;  // pointer to float value obtained via syntax *in 
                          // and then increment the "in" pointer by one elment 
                         // (by 4 bytes via only ++ memory address bytes with float "in" buffer)
              ++in;
              d_b.d_noisy_symbols.push_back(arr[j]);
              d_b.d_noisy_symbols[j] = -d_b.d_noisy_symbols[j];
              }

              if(j>2047)
              {
              arr[j] = extra0;  // set last 512 to zero
              d_b.d_noisy_symbols.push_back(arr[j]);
              }
           }    // Frame ready for decoding

           // Do the decoding
           d_m.d_modem->demodulate(d_b.d_sigma, d_b.d_noisy_symbols, d_b.d_LLRs);
           int ret = d_m.d_decoder->decode_siho(d_b.d_LLRs, d_b.d_dec_bits); //calculates decode bits/frame
  // Note: decoded bits are (char or Byte) type unpacked.
           if (ret)
           {
              std::cerr << "failed to decode frame" << std::endl;
              ++d_failed_frames;
    std::cout << " frames (" << d_failed_frames << " failed)" << std::endl;
           }
           else
           {
              std::cerr << "PASSED SUCCESSFUL decoded frame" << std::endl;
              ++d_passed_frames;
    std::cout << " frames (" << d_passed_frames << " passed)" << std::endl;
           }

           // Pack the bits
           uint8_t x = 0;
           for (int t = 0; t < 1024; ++t)  // p.K is 1024 and original length of frames
           {
               x = (x << 1) | d_b.d_dec_bits[t];  // via decoded bits
               if (t % 8 == 7) {
                d_b.d_dec_bits_packed[t / 8] = x;
               }
           }

    std::copy(d_b.d_dec_bits_packed.begin(), d_b.d_dec_bits_packed.end(), arrout);
           for (int s = 0; s < 128; s++)  //  2048/16 = 128 (char output)
           {
              *out = arrout[s];
              ++out;
           }       // END of "i" LOOP

      }       // END of "i" LOOP


    consume_each(n_digested);//  noutput_items);

    // Tell runtime system how many output items we produced.
    return n_produced; //noutput_items;
}

} /* namespace aff3ct_codes */
} /* namespace gr */
