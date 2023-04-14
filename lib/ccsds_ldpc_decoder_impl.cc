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

      size_t d_failed_frames = 0;
      size_t d_passed_frames = 0;
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
      char* out = (char*)output_items[0];
      //float* out = (float*)output_items[0]; // need to make output a "char/byte"

//  TEST Code from AFF3CT
    //  Use the aff3ct buffers - fill via memcpy of gnuradio circular input buffer rather than "fill" functions below.
   
      uint64_t n_digested = 20*2048; // 12 frames for frame length 2048 
      uint64_t n_produced = 20*2048/16; // 12 frames for frame length 2048 
      //size_t failed_frames = 0;
      //size_t passed_frames = 0;
              // Initialise an array for b.noisy
      float arr[2560];
      float extra0 = 0.0;
      char arrout[128];

    struct params {
        int K = 1024;           // number of information bits
        int N = 2048 + 512;     // codeword size
        int puncturing = 512;   // number of punctured bits
        int n_ite = 40;        // number of decoder iterations
        aff3ct::tools::Sparse_matrix H; // parity check matrix
        std::vector<uint32_t> info_bits_pos;  //position in container for H
        // The maximum check-node degree for the AR4JA code used is 6
        Update_rule_SPA<> update_rule = aff3ct::tools::Update_rule_SPA<>(6);
    };

    params p;
    //const char* alist_path = "alists/ccsds_ar4ja_r1_2_k1024.alist";   //path and name of alist file
    std::ifstream alist_file;  //set file name object for in-out
    alist_file.open("ccsds_ar4ja_r1_2_k1024.alist", std::ifstream::in);
    p.H = aff3ct::tools::AList::read(alist_file);
    for (int j = 0; j < p.K; ++j) {
        p.info_bits_pos.push_back(j);
    }

    struct modules {
        std::unique_ptr<aff3ct::module::Modem_BPSK<>> modem;
        std::unique_ptr<aff3ct::module::Decoder_LDPC_BP_flooding<>> decoder;
    };
    modules m;
    m.modem = std::unique_ptr<aff3ct::module::Modem_BPSK<>>(new aff3ct::module::Modem_BPSK<>(p.N));
    m.decoder = std::unique_ptr<aff3ct::module::Decoder_LDPC_BP_flooding<>>(new aff3ct::module::Decoder_LDPC_BP_flooding<>(p.K, p.N, p.n_ite, p.H, p.info_bits_pos, p.update_rule));
    // "NEW": is used to give dynamic memory allocation when size of memory is variable
    // - must release memory or memory leak until program closed.

    struct buffers {
        std::vector<float> sigma;
        std::vector<float> noisy_symbols;
        std::vector<float> LLRs;
        std::vector<int> dec_bits;
        std::vector<uint8_t> dec_bits_packed;
    };

      buffers b;
      b.sigma = std::vector<float>(1);
      b.noisy_symbols = std::vector<float>(p.N);
      b.LLRs = std::vector<float>(p.N);
      b.dec_bits = std::vector<int>(p.K);
      b.dec_bits_packed = std::vector<uint8_t>(p.K / 8);


      std::fill(b.sigma.begin(), b.sigma.end(), 0.5);
              // "fill" from .begin to .end all set to 0.5 for each element/item

      for(int i=0; i<20; i++)  // each 2048 symbol frame in one WORK function
      {
           b.noisy_symbols.clear();  //container reset 0 size for each "i" increment 
                 // erase() is also an option from begin to somehere else container
           // Traverse the in/out into/outof array and d_b.d_noisy_symbols prior to 512 zeros
           for (int j = 0; j < 2560; j++)
           {
              if(j<2048)
              {
              arr[j] = *in;  // pointer in points to float value obtained via syntax *in 
                          // and the increment the "in" pointer by one elment 
                         // (by 4 via ++ memory address bytes)
              ++in;
              b.noisy_symbols.push_back(arr[j]);
              b.noisy_symbols[j] = -b.noisy_symbols[j];
              //*out = b.noisy_symbols[j];
              //++out;
    //std::cout << " NOISY SYMBOLS (" << b.noisy_symbols[j] << " failed)" << std::endl;
              }
              if(j>2047)
              {
              arr[j] = extra0;  // pointer in points to float value obtained via syntax *in 
                          // and the increment the "in" pointer by one elment 
                         // (by 4 via ++ memory address bytes)
              b.noisy_symbols.push_back(arr[j]);
    //std::cout << " ZERO NOISY SYMBOLS (" << b.noisy_symbols[j] << " failed)" << std::endl;
              }
           }    // Frame ready for decoding

           // Do the decoding
           m.modem->demodulate(b.sigma, b.noisy_symbols, b.LLRs);
           int ret = m.decoder->decode_siho(b.LLRs, b.dec_bits); //calculates decode bits/frame
  // Note: decoded bits are "int" (char or Byte) type unpacked.
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

           uint8_t x = 0;
           for (int t = 0; t < 1024; ++t)  // p.K is 1024 and original length of frames
           {
               x = (x << 1) | b.dec_bits[t];  // via decoded bits
               if (t % 8 == 7) {
                b.dec_bits_packed[t / 8] = x;
               }
           }

    std::copy(b.dec_bits_packed.begin(), b.dec_bits_packed.end(), arrout);
           for (int s = 0; s < 128; s++)
           {
              *out = arrout[s];
              ++out;
           }       // END of "i" LOOP

      }       // END of "i" LOOP


    //for (int k = 0; k < noutput_items; k++) {
        //out[k] = in[k];
    //}

    consume_each(n_digested);//  noutput_items);

    // Tell runtime system how many output items we produced.
    return n_produced; //noutput_items;
}

} /* namespace aff3ct_codes */
} /* namespace gr */
