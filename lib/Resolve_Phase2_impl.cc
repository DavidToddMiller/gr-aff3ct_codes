/* -*- c++ -*- */
/*
 * Copyright 2023 David T Miller.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "Resolve_Phase2_impl.h"
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
Resolve_Phase2::sptr
Resolve_Phase2::make(int waveform, int frame_length, int buffer_length, int threshold)
{
    return gnuradio::make_block_sptr<Resolve_Phase2_impl>(
        waveform, frame_length, buffer_length, threshold);
}


/*
 * The private constructor
 */
Resolve_Phase2_impl::Resolve_Phase2_impl(int waveform,
                                         int frame_length,
                                         int buffer_length,
                                         int threshold)
    : gr::block("Resolve_Phase2",
                gr::io_signature::make(
                    1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                gr::io_signature::make(
                    1 /* min outputs */, 1 /*max outputs */, sizeof(output_type))),
       // CCSDS 32 Bit ASM Resolution BPSK and QPSK
 
   s{0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0},
           // 64 bit CCSDS ASM
      //s{0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1},
  s90{1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0}, // 64 bit CCSDS ASM
      // s90{1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0},
      d_waveform(waveform),
      d_frame_length(2112), // 
      d_buffer_length(30000), // 30000 items-samples for 15 Mbps and 15000 for low rate CCSDS
      d_threshold(threshold)
    {
        set_tag_propagation_policy(TPP_DONT); 
        set_output_multiple(30000); 
    }

/*
 * Our virtual destructor.
 */
Resolve_Phase2_impl::~Resolve_Phase2_impl() {}

void Resolve_Phase2_impl::forecast(int noutput_items,
                                   gr_vector_int& ninput_items_required)
{
 ninput_items_required[0] = noutput_items;
}

int Resolve_Phase2_impl::general_work(int noutput_items,
                                      gr_vector_int& ninput_items,
                                      gr_vector_const_void_star& input_items,
                                      gr_vector_void_star& output_items)
    {
      uint64_t n_digested = 12*d_frame_length; // 13 frames for frame length 2112 
      uint64_t n_produced = 12*d_frame_length; // 13 frames for frame length 2112 
      int d_phase = 45;
      int d_compare = 0;
      int d_compare90 = 0;

      const float* in = (const float*)input_items[0];
      float* out = (float*)output_items[0];

      for(int i=0; i<12; i++) {
           
           // ASM determination of phase first as 45 deg, 135 deg, 225 deg, 315 deg for QPSK
           for(int j=0; j<64; j++) {
              float y = (float)(s[j])*2-1;
              if( in[j+i*2112] * y > 0)
              {
                 d_compare += 1;
              }
           } // End of j FOR loop

               GR_LOG_DEBUG(d_logger, boost::format("d_compare %llu") % (d_compare));
               GR_LOG_DEBUG(d_logger, boost::format("d_compare90 %llu") % (d_compare90));
           if(d_compare >= 64-d_threshold)  //threshold set to 1 error
           {    
              d_phase = 45;  //45 degrees lock
              GR_LOG_DEBUG(d_logger, boost::format("Resolve Block PHASE in DEGREES %llu") % (d_phase));
           }
           else if(d_compare <= d_threshold)  //threshold set to 1 error
           {    
              d_phase = 225;  //225 degrees lock so need to flip bits
              GR_LOG_DEBUG(d_logger, boost::format("Resolve Block PHASE in DEGREES %llu") % (d_phase));
           }
           else if(d_compare90 >= 64-d_threshold)  //threshold set to 1 error
           {    
              d_phase = 135;  //135 degrees lock
              GR_LOG_DEBUG(d_logger, boost::format("Resolve Block PHASE in DEGREES %llu") % (d_phase));
           }
           else if(d_compare90 <= d_threshold)  //threshold set to 1 error
           {    
              d_phase = 315;  //315 degrees lock so need to flip bits
              GR_LOG_DEBUG(d_logger, boost::format("Resolve Block PHASE in DEGREES %llu") % (d_phase));
           }
           else
           {
              d_phase = 315;  //315 degrees lock so need to flip bits
              GR_LOG_DEBUG(d_logger, boost::format("DEFAULT 45: Resolve Block PHASE in DEGREES %llu") % (d_phase));
           }
           //GR_LOG_DEBUG(d_logger, boost::format("Resolve Block PHASE in DEGREES %llu") % (d_phase));
               //GR_LOG_DEBUG(d_logger, boost::format("d_compare %llu") % (d_compare));
               //GR_LOG_DEBUG(d_logger, boost::format("d_compare90 %llu") % (d_compare90));
           d_compare = 0;  //reset d_compare for each run through 13 ASMs per WORK call
           d_compare90 = 0;  //reset d_compare for each run through 13 ASMs per WORK call

           for(int m=0; m<2112; m++) 
           {      // rotate frame and its ASM and output it
              switch(d_phase) //{   //use int d_waveform = 1 if BPSK and d_waveform = 2 if QPSK
              {   // bracket for "d_phase" switch
                 case 45:        //for 45 degree phase, no rotation and so no flip bits, just output it
                   out[m+i*2112] = in[m+i*2112];  
                   out[m+1+i*2112] = in[m+1+i*2112];  
                   m += 1;                
                 break; 

                 case 225:        //for 225 deg, 180 deg rotation - flip all bits
                   out[m+i*2112] = -in[m+i*2112] ; 
                   out[m+1+i*2112] = -in[m+1+i*2112] ;                 
                   m += 1;    
                 break;

                 case 135:     //for 135 deg, 90 deg rotation so exactly half bits will be flipped only 
                   if((in[m+i*2112] > 0.0) && (in[m+1+i*2112] > 0.0))
                   {
                      out[m+i*2112] = in[m+i*2112];
                      out[m+1+i*2112] = -in[m+1+i*2112];
                   }
                   else if((in[m+i*2112] < 0.0) && (in[m+1+i*2112] > 0.0))
                   {
                      out[m+i*2112] = -in[m+i*2112];
                      out[m+1+i*2112] = in[m+1+i*2112];
                   }
                   else if((in[m+i*2112] < 0) && (in[m+1+i*2112] < 0))
                   {
                      out[m+i*2112] = in[m+i*2112];
                      out[m+1+i*2112] = -in[m+1+i*2112];
                   }
                   else // must be (in[m+i*frame_length] == 1 && in[m+1+i*frame_length] == 0)
                   {
                      out[m+i*2112] = -in[m+i*2112];
                      out[m+1+i*2112] = in[m+1+i*2112];
                   }
                   m += 1;    
                 break;

                 case 315:     //for 315 deg, 270 deg rotation so exactly half bits will be flipped only 
                   if((in[m+i*2112] > 0.0) && (in[m+1+i*2112] > 0.0))
                   {
                      out[m+i*2112] = in[m+i*2112];
                      out[m+1+i*2112] = -in[m+1+i*2112];
                   }
                   else if((in[m+i*2112] < 0) && (in[m+1+i*2112] > 0.0))
                   {
                      out[m+i*2112] = in[m+i*2112];
                      out[m+1+i*2112] = -in[m+1+i*2112];
                   }
                   else if((in[m+i*2112] < 0.0) && (in[m+1+i*2112] < 0.0))
                   {
                      out[m+i*2112] = -in[m+i*2112];
                      out[m+1+i*2112] = in[m+1+i*2112];
                   }
                   else  // must be (in[m+i*4192] == 1 && in[m+1+i*4192] == 0)
                   {
                      out[m+i*2112] = in[m+i*2112];
                      out[m+1+i*2112] = -in[m+1+i*2112];
                   }
                   m += 1;
                 break;

              } // end of SWITCH function for d_phase
            } // End of m FOR loop for each frame
    }   

      consume_each (n_digested);   //tell scheduler runtime the amount of input items consum     
      return n_produced;    //tell scheduler runtime output items
    }    // end of WORK function


} /* namespace aff3ct_codes */
} /* namespace gr */
