/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_AFF3CT_CODES_CCSDS_PSEUDO_RANDOMIZER_H
#define INCLUDED_AFF3CT_CODES_CCSDS_PSEUDO_RANDOMIZER_H

#include <gnuradio/aff3ct_codes/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace aff3ct_codes {

/*!
 * \brief <+description of block+>
 * \ingroup aff3ct_codes
 *
 */
class AFF3CT_CODES_API CCSDS_Pseudo_Randomizer : virtual public gr::block
{
public:
    typedef std::shared_ptr<CCSDS_Pseudo_Randomizer> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of
     * aff3ct_codes::CCSDS_Pseudo_Randomizer.
     *
     * To avoid accidental use of raw pointers, aff3ct_codes::CCSDS_Pseudo_Randomizer's
     * constructor is in a private implementation
     * class. aff3ct_codes::CCSDS_Pseudo_Randomizer::make is the public interface for
     * creating new instances.
     */
    static sptr make(int length, int count, int bits_per_byte);
};

} // namespace aff3ct_codes
} // namespace gr

#endif /* INCLUDED_AFF3CT_CODES_CCSDS_PSEUDO_RANDOMIZER_H */
