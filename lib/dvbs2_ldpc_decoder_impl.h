/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_AFF3CT_CODES_DVBS2_LDPC_DECODER_IMPL_H
#define INCLUDED_AFF3CT_CODES_DVBS2_LDPC_DECODER_IMPL_H

#include <gnuradio/aff3ct_codes/dvbs2_ldpc_decoder.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>

#include <aff3ct.hpp>

namespace gr {
namespace aff3ct_codes {

class dvbs2_ldpc_decoder_impl : public dvbs2_ldpc_decoder
{
private:
    // Nothing to declare in this block.
      size_t d_failed_frames;
      size_t d_passed_frames;

      int d_K;
      int d_N;     // codeword size
      int d_puncturing;   // number of punctured bits
      int d_n_ite;        // number of decoder iterations
      Update_rule_SPA<> d_update_rule = aff3ct::tools::Update_rule_SPA<>(6);
      std::vector<uint32_t> d_info_bits_pos;
      aff3ct::tools::Sparse_matrix d_H; // parity check matrix
      std::ifstream d_alist_file;  //set file name object for in-out

    struct modules {
        std::unique_ptr<aff3ct::module::Modem_BPSK<>> d_modem;
        std::unique_ptr<aff3ct::module::Decoder_LDPC_BP_flooding<>> d_decoder;
    };
    struct modules d_m;

    struct buffers {
        std::vector<float> d_sigma;
        std::vector<float> d_noisy_symbols;
        std::vector<float> d_LLRs;
        std::vector<int> d_dec_bits;
        std::vector<uint8_t> d_dec_bits_packed;
    };
    struct buffers d_b;


public:
    dvbs2_ldpc_decoder_impl(int code_rate, bool data_rate, int decoder_iterations);
    ~dvbs2_ldpc_decoder_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace aff3ct_codes
} // namespace gr

#endif /* INCLUDED_AFF3CT_CODES_DVBS2_LDPC_DECODER_IMPL_H */
