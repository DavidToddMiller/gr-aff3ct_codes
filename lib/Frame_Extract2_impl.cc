/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "Frame_Extract2_impl.h"
#include <gnuradio/io_signature.h>

#include <volk/volk.h>
#include <boost/format.hpp>
#include <cstdio>
#include <iostream>
#include <stdexcept>

#include <memory>
#include <string>
#include <vector>

namespace gr {
namespace aff3ct_codes {

using input_type = float;
using output_type = float;
Frame_Extract2::sptr
Frame_Extract2::make(int frame_length, int buffer_length, int ASM_length)
{
    return gnuradio::make_block_sptr<Frame_Extract2_impl>(
        frame_length, buffer_length, ASM_length);
}


/*
 * The private constructor
 */
Frame_Extract2_impl::Frame_Extract2_impl(int frame_length,
                                         int buffer_length,
                                         int ASM_length)
    : gr::block("Frame_Extract2",
                gr::io_signature::make(
                    1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                gr::io_signature::make(
                    1 /* min outputs */, 1 /*max outputs */, sizeof(output_type))),
      d_frame_length(frame_length), // 
      d_buffer_length(buffer_length), // 15000 for small size frames (<3000 bits)
      d_ASM_length(ASM_length)
    {
        set_tag_propagation_policy(TPP_DONT); 
        set_output_multiple(30000); 
        n_dropped_times = 0;
    }

/*
 * Our virtual destructor.
 */
Frame_Extract2_impl::~Frame_Extract2_impl() {}

void Frame_Extract2_impl::forecast(int noutput_items,
                                   gr_vector_int& ninput_items_required)
{
 ninput_items_required[0] = noutput_items;
}

int Frame_Extract2_impl::general_work(int noutput_items,
                                      gr_vector_int& ninput_items,
                                      gr_vector_const_void_star& input_items,
                                      gr_vector_void_star& output_items)
    {
      //Remove tags from frames not "d_frame_length" bits in length that come from chunk chain discontinuities or ccsds flowgraph frame length
      uint64_t n_digested = 0; // set max for circular buffer to use
      uint64_t n_produced = 0;  // set max for circular buffer to use

      const float* in = (const float*)input_items[0]; //defines the void pointers to float
      float* out = (float*)output_items[0];

      std::vector<tag_t> tags;
      get_tags_in_range(tags, 0, nitems_read(0) + 64, nitems_read(0) + noutput_items);
      GR_LOG_DEBUG(d_logger, boost::format("writing tag size %llu") % (tags.size()));

      if (int(tags.size())<5)  // STOP and move on to next 30000 bits in next WORK Call
      {
      n_digested = 5000;  // 10000 for hdr 15.0 Mbps flowgraph but only 5000 for CCSDS low rate flowgraph
      n_produced = 0;  // 10000 for hdr 15.0 Mbps flowgraph but only 5000 for CCSDS low rate flowgraph
      }

      if (int(tags.size())>4)  // Extract frames in WORK Call via ASM and maintain ASM
      {
         n_digested = tags[0].offset-nitems_read(0)-64;//start point: function of ASM length
         for (int n = 0; n < (int)n_digested; n++)
         {
              //CCSDS 2112 with 64 bit ASM; 
             ++in;  
         }         //in += 4*n_digested;  // "in" is a pointer and 4 bytes per float item to increment

         int tags_length = int(tags.size()-3);  // -2 so no partial frames in WORK call extracted

         for(int i=0; i<tags_length; i++)
         {
            //int offset = int(tags[i].offset);
            int offset_start = int(tags[i].offset);
            int offset_end = int(tags[i+1].offset);
            int delta = offset_end - offset_start;
            int x = 2112;
            float arrin[2112];
            float arrout[2112];
            if (delta == x)
            {
                //uint64_t n_in = n_digested * 4; // set max for circular buffer to use
                //uint64_t n_out = n_produced * 4;             
                //memcpy((void*)(out + n_out), (const void*)(in + n_in), 8448); 
                //n_digested += 2112;  //CCSDS 2112 with 64 bit ASM; 
                  //in = in + sizeof(float);
                //n_produced += 2112; //CCSDS 2112 with 64 bit ASM;  
                 for (int s = 0; s<x; s++)
                 {
                    //*out = *in;

                    arrin[s] = *in;  // pointer in points to float value obtained via syntax *in 
                    arrout[s] = arrin[s];
                    *out = arrout[s];
                    n_digested += 1;  //CCSDS 2112 with 64 bit ASM; 
                    ++in; //= in + sizeof(float);
                    n_produced += 1; //CCSDS 2112 with 64 bit ASM;  
                    ++out;  // = out + sizeof(float);
                 }       //

               //memcpy((void*) out, (const void*) in, 2112);   // 4 memory bytes per item - 4 * 2112

               //n_digested += delta;  //CCSDS 2112 with 64 bit ASM; 
               //in += 4*2112;              
               //n_produced += delta; //CCSDS 2112 with 64 bit ASM;  
               //out += 4*2112;   
            }
            if(delta!=x)  // frame length is other length 2112 or false frame lock
            {
                 for (int p = 0; p<delta ; p++)
                 {
                   n_digested += 1;  //CCSDS 2112 with 64 bit ASM; 
                   ++in;   // = in + sizeof(float);
                 }       //
//                  n_digested += delta;  //CCSDS 64;                  
//                 in += delta;           
            }
         }  // END of FOR loop with "i"

      //GR_LOG_DEBUG(d_logger, boost::format("DROPPED incrementer value %llu") % (n_dropped_times));

      }  // END of "IF" with tag size <5 

      consume_each (n_digested);   //tell scheduler runtime the amount of input items consum     
      return n_produced;    //tell scheduler runtime output items
    }


} /* namespace aff3ct_codes */
} /* namespace gr */
