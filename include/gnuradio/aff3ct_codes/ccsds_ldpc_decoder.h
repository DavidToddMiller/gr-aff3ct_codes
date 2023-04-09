/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_AFF3CT_CODES_CCSDS_LDPC_DECODER_H
#define INCLUDED_AFF3CT_CODES_CCSDS_LDPC_DECODER_H

#include <gnuradio/aff3ct_codes/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace aff3ct_codes {

/*!
 * \brief <+description of block+>
 * \ingroup aff3ct_codes
 *
 */
class AFF3CT_CODES_API ccsds_ldpc_decoder : virtual public gr::block
{
public:
    typedef std::shared_ptr<ccsds_ldpc_decoder> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of aff3ct_codes::ccsds_ldpc_decoder.
     *
     * To avoid accidental use of raw pointers, aff3ct_codes::ccsds_ldpc_decoder's
     * constructor is in a private implementation
     * class. aff3ct_codes::ccsds_ldpc_decoder::make is the public interface for
     * creating new instances.
     */
    static sptr make(int code_rate, bool data_rate, int decoder_iterations);
};

} // namespace aff3ct_codes
} // namespace gr

#endif /* INCLUDED_AFF3CT_CODES_CCSDS_LDPC_DECODER_H */
