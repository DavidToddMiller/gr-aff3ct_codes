/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
 
#include "dvbs2_ldpc_decoder_impl.h"
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
dvbs2_ldpc_decoder::sptr
dvbs2_ldpc_decoder::make(int code_rate, bool data_rate, int decoder_iterations)
{
    return gnuradio::make_block_sptr<dvbs2_ldpc_decoder_impl>(
        code_rate, data_rate, decoder_iterations);
}


/*
 * The private constructor
 */
dvbs2_ldpc_decoder_impl::dvbs2_ldpc_decoder_impl(int code_rate,
                                                 bool data_rate,
                                                 int decoder_iterations)
    : gr::block("dvbs2_ldpc_decoder",
                gr::io_signature::make(
                    1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                gr::io_signature::make(
                    1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
{
    set_output_multiple(80000); 
       
      // Basic AFF3CT LDPC Parameters
      d_K = 32400;
      d_N = 64800;     // codeword size
      d_puncturing = 0;   // number of punctured bits
      d_n_ite = 30;        // number of decoder iterations

      for (int j = 0; j < d_K; ++j) {
        d_info_bits_pos.push_back(j);
      }

      // Construct LDPC H Matrix via alist
      //d_alist_file;  //set file name object for in-out
      d_alist_file.open("H_matrix_dvbs2_r1_2_k32400.alist", std::ifstream::in);
      d_H = aff3ct::tools::AList::read(d_alist_file);


     // Modem-Decoder struct Object parameters
      d_m.d_modem = std::unique_ptr<aff3ct::module::Modem_BPSK<>>(new aff3ct::module::Modem_BPSK<>(d_N));
      d_m.d_decoder = std::unique_ptr<aff3ct::module::Decoder_LDPC_BP_flooding<>>(new aff3ct::module::Decoder_LDPC_BP_flooding<>(d_K, d_N, d_n_ite, d_H, d_info_bits_pos, d_update_rule));

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
dvbs2_ldpc_decoder_impl::~dvbs2_ldpc_decoder_impl() {}

void dvbs2_ldpc_decoder_impl::forecast(int noutput_items,
                                       gr_vector_int& ninput_items_required)
{
 ninput_items_required[0] = noutput_items;
}

int dvbs2_ldpc_decoder_impl::general_work(int noutput_items,
                                          gr_vector_int& ninput_items,
                                          gr_vector_const_void_star& input_items,
                                          gr_vector_void_star& output_items)
{
      const float* in = (const float*)input_items[0];  //Each float item takes up 4 bytes in memory
      char* out = (char*)output_items[0]; //char hard decision output after soft decision decoding
  
      uint64_t n_digested = 1*64800; // 1 frame for frame length 64800 
      uint64_t n_produced = 1*64800/16; // char output packed and associated rate 1/2
 
         // Initialise an array for b.noisy in/out interface with GNU Radio block circular buffers
      float arr[64800];
      float extra0 = 0.0;
      char arrout[4050];  //for dvbs2 packed output

      std::fill(d_b.d_sigma.begin(), d_b.d_sigma.end(), 0.5);
              // "fill" from .begin to .end all set to 0.5 for each element/item

      for(int i=0; i<1; i++)  // each 64000 symbol frame in one WORK function
      {
           d_b.d_noisy_symbols.clear();  //container reset 0 size for each "i" increment 
            // Traverse the in/out into/outof array and d_b.d_noisy_symbols - no punctured bits-zeros
           for (int j = 0; j < 64800; j++)
           {
              if(j<64800)
              {
              arr[j] = *in;  // pointer to float value obtained via syntax *in 
                          // and then increment the "in" pointer by one elment 
                         // (by 4 bytes via only ++ memory address bytes with float "in" buffer)
              ++in;
              d_b.d_noisy_symbols.push_back(arr[j]);
              d_b.d_noisy_symbols[j] = -d_b.d_noisy_symbols[j];
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
           for (int t = 0; t < 32400; ++t)  // p.K is 32400 and original length of dvbs2 rate 1/2 frame
           {
               x = (x << 1) | d_b.d_dec_bits[t];  // via decoded bits
               if (t % 8 == 7) {
                d_b.d_dec_bits_packed[t / 8] = x;
               }
           }

    std::copy(d_b.d_dec_bits_packed.begin(), d_b.d_dec_bits_packed.end(), arrout);
           for (int s = 0; s < 4050; s++)  //  32400/8 = 4050 (char output) packed
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
