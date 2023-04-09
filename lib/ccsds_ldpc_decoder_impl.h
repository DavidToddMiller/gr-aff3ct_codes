/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_AFF3CT_CODES_CCSDS_LDPC_DECODER_IMPL_H
#define INCLUDED_AFF3CT_CODES_CCSDS_LDPC_DECODER_IMPL_H

#include <gnuradio/aff3ct_codes/ccsds_ldpc_decoder.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>

#include <aff3ct.hpp>

namespace gr {
namespace aff3ct_codes {

class ccsds_ldpc_decoder_impl : public ccsds_ldpc_decoder
{
private:
    // Nothing to declare in this block.
      size_t d_failed_frames;
      size_t d_passed_frames;

    struct params {
        int d_K;           // number of information bits
        int d_N;     // codeword size
        int d_puncturing;   // number of punctured bits
        int d_n_ite;        // number of decoder iterations
        aff3ct::tools::Sparse_matrix d_H; // parity check matrix
        std::vector<uint32_t> d_info_bits_pos;
        // The maximum check-node degree for the AR4JA code used is 6
        Update_rule_SPA<> update_rule = aff3ct::tools::Update_rule_SPA<>(6);
    };

    struct params d_p;
    //const char* alist_path;
    std::ifstream alist_file;  //set file name object for in-out
    //alist_file.open("ccsds_ar4ja_r1_2_k1024.alist", std::ifstream::in);
    //p.H = aff3ct::tools::AList::read(alist_file);

    struct modules {
        std::unique_ptr<aff3ct::module::Modem_BPSK<>> d_modem;
        std::unique_ptr<aff3ct::module::Decoder_LDPC_BP_flooding<>> d_decoder;
    };
    struct params d_m;

    struct buffers {
        std::vector<float> d_sigma;
        std::vector<float> d_noisy_symbols;
        std::vector<float> d_LLRs;
        std::vector<int> d_dec_bits;
        std::vector<uint8_t> d_dec_bits_packed;
    };
    struct buffers d_b;

public:
    ccsds_ldpc_decoder_impl(int code_rate, bool data_rate, int decoder_iterations);
    ~ccsds_ldpc_decoder_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace aff3ct_codes
} // namespace gr

#endif /* INCLUDED_AFF3CT_CODES_CCSDS_LDPC_DECODER_IMPL_H */
