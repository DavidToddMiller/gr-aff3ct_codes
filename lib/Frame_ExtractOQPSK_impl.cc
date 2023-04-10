/* -*- c++ -*- */
/*
 * Copyright 2023 D Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "Frame_ExtractOQPSK_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace aff3ct_codes {

using input_type = float;
using output_type = float;
Frame_ExtractOQPSK::sptr
Frame_ExtractOQPSK::make(int frame_length, int buffer_length, int ASM_length)
{
    return gnuradio::make_block_sptr<Frame_ExtractOQPSK_impl>(
        frame_length, buffer_length, ASM_length);
}


/*
 * The private constructor
 */
Frame_ExtractOQPSK_impl::Frame_ExtractOQPSK_impl(int frame_length,
                                                 int buffer_length,
                                                 int ASM_length)
    : gr::block("Frame_ExtractOQPSK",
                gr::io_signature::make(
                    2 /* min inputs */, 2 /* max inputs */, sizeof(input_type)),
                gr::io_signature::make(
                    1 /* min outputs */, 1 /*max outputs */, sizeof(output_type))),
      d_frame_length(2112), // 
      d_buffer_length(30000), // 15000 for small size frames (<3000 bits)
      d_ASM_length(64)
    {
        set_tag_propagation_policy(TPP_DONT); 
        set_output_multiple(30000); 
        n_dropped_times = 0;
    }

/*
 * Our virtual destructor.
 */
Frame_ExtractOQPSK_impl::~Frame_ExtractOQPSK_impl() {}

void Frame_ExtractOQPSK_impl::forecast(int noutput_items,
                                       gr_vector_int& ninput_items_required)
    {
    ninput_items_required[0] = noutput_items;
    ninput_items_required[1] = noutput_items;
    }


int Frame_ExtractOQPSK_impl::general_work(int noutput_items,
                                          gr_vector_int& ninput_items,
                                          gr_vector_const_void_star& input_items,
                                          gr_vector_void_star& output_items)
    {
      //Remove tags from frames not "d_frame_length" bits in length that come from chunk chain discontinuities or ccsds flowgraph frame length
      uint64_t n_digested0 = 0;
      uint64_t n_digested1 = 0;
      uint64_t n_digested = 0;
      uint64_t n_produced0 = 0;
      uint64_t n_produced1 = 0;
      uint64_t n_produced = 0;

      const float* in0 = (const float*)input_items[0]; //defines the void pointers to float
      const float* in1 = (const float*)input_items[1]; //defines the void pointers to float
      float* out = (float*)output_items[0];

// INPUT #0 --------------------------------------------
      std::vector<tag_t> tags0;
      get_tags_in_range(tags0, 0, nitems_read(0) + 64, nitems_read(0) + noutput_items);
      GR_LOG_DEBUG(d_logger, boost::format("writing tag 0 size %llu") % (tags0.size()));

      if (int(tags0.size())<5)  // STOP and move on to next 30000 bits in next WORK Call
      {
      n_digested0 = 5000;  // 10000 for hdr 15.0 Mbps flowgraph but only 5000 for CCSDS low rate flowgraph
      n_produced0 = 0;  // 10000 for hdr 15.0 Mbps flowgraph but only 5000 for CCSDS low rate flowgraph
      }

      if (int(tags0.size())>4)  // Extract frames in WORK Call via ASM and maintain ASM
      {
         n_digested0 = tags0[0].offset-nitems_read(0)-64;//start point: function of ASM length
         for (int n = 0; n < (int)n_digested0; n++)
         {
              //CCSDS 2112 with 64 bit ASM; 
             ++in0;  
         }         //in += 4*n_digested;  // "in" is a pointer and 4 bytes per float item to increment

         int tags_length0 = int(tags0.size()-3);  // -2 so no partial frames in WORK call extracted

         for(int i=0; i<tags_length0; i++)
         {
            //int offset = int(tags[i].offset);
            int offset_start0 = int(tags0[i].offset);
            int offset_end0 = int(tags0[i+1].offset);
            int delta0 = offset_end0 - offset_start0;
            int x0 = 2112;
            float arrin0[2112];
            float arrout0[2112];
            if (delta0 == x0)
            {
                 for (int s = 0; s<x0; s++)
                 {
                    arrin0[s] = *in0;  // pointer in points to float value obtained via syntax *in 
                    arrout0[s] = arrin0[s];
                    *out = arrout0[s];
                    n_digested0 += 1;  //CCSDS 2112 with 64 bit ASM; 
                    ++in0; //= in + sizeof(float);
                    n_produced0 += 1; //CCSDS 2112 with 64 bit ASM;  
                    ++out;  // = out + sizeof(float);
                 }       //
            }

            if(delta0!=x0)  // frame length is other length 2112 or false frame lock
            {
                 for (int p = 0; p<delta0 ; p++)
                 {
                   n_digested0 += 1;  //CCSDS 2112 with 64 bit ASM; 
                   ++in0;   // = in + sizeof(float);
                 }       //
//                  n_digested += delta;  //CCSDS 64;                  
//                 in += delta;           
            }
         }  // END of FOR loop with "i"

      //GR_LOG_DEBUG(d_logger, boost::format("DROPPED incrementer value %llu") % (n_dropped_times));

      }  // END of "IF" with tag size <5 

// INPUT #1 --------------------------------------------
      std::vector<tag_t> tags1;
      get_tags_in_range(tags1, 1, nitems_read(1) + 64, nitems_read(1) + noutput_items);
      GR_LOG_DEBUG(d_logger, boost::format("writing tag 1 size %llu") % (tags1.size()));

      if (int(tags1.size())<5)  // STOP and move on to next 30000 bits in next WORK Call
      {
      n_digested1 = 5000;  // 10000 for hdr 15.0 Mbps flowgraph but only 5000 for CCSDS low rate flowgraph
      n_produced1 = 0;  // 10000 for hdr 15.0 Mbps flowgraph but only 5000 for CCSDS low rate flowgraph
      }

      if (int(tags1.size())>4)  // Extract frames in WORK Call via ASM and maintain ASM
      {
         n_digested1 = tags1[0].offset-nitems_read(1)-64;//start point: function of ASM length
         for (int k = 0; k < (int)n_digested1; k++)
         {
              //CCSDS 2112 with 64 bit ASM; 
             ++in1;  
         }         //in += 4*n_digested;  // "in" is a pointer and 4 bytes per float item to increment

         int tags_length1 = int(tags1.size()-3);  // -2 so no partial frames in WORK call extracted

         for(int m=0; m<tags_length1; m++)
         {
            //int offset = int(tags[i].offset);
            int offset_start1 = int(tags1[m].offset);
            int offset_end1 = int(tags1[m+1].offset);
            int delta1 = offset_end1 - offset_start1;
            int x1 = 2112;
            float arrin1[2112];
            float arrout1[2112];
            if (delta1 == x1)
            {
                 for (int q = 0; q<x1; q++)
                 {
                    arrin1[q] = *in1;  // pointer in points to float value obtained via syntax *in 
                    arrout1[q] = arrin1[q];
                    *out = arrout1[q];
                    n_digested1 += 1;  //CCSDS 2112 with 64 bit ASM; 
                    ++in1; //= in + sizeof(float);
                    n_produced1 += 1; //CCSDS 2112 with 64 bit ASM;  
                    ++out;  // = out + sizeof(float);
                 }       //
            }

            if(delta1!=x1)  // frame length is other length 2112 or false frame lock
            {
                 for (int r = 0; r<delta1 ; r++)
                 {
                   n_digested1 += 1;  //CCSDS 2112 with 64 bit ASM; 
                   ++in1;   // = in + sizeof(float);
                 }       //
//                  n_digested += delta;  //CCSDS 64;                  
//                 in += delta;           
            }
         }  // END of FOR loop with "i"

      //GR_LOG_DEBUG(d_logger, boost::format("DROPPED incrementer value %llu") % (n_dropped_times));

      }  // END of "IF" with tag size <5 

     n_digested = std::max(n_digested0, n_digested1);
     n_produced = n_produced0 + n_produced1;

     consume (0, n_digested);   //tell scheduler runtime the amount of input items consumed
     consume (1, n_digested);   //tell scheduler runtime the amount of input items consumed


      //consume_each (n_digested);   //tell scheduler runtime the amount of input items consum     
      return n_produced;    //tell scheduler runtime output items
    }

} /* namespace aff3ct_codes */
} /* namespace gr */
