                                  gr-aff3ct_codes

gr-aff3ct_codes is a GNU Radio module set of blocks that incorporates a C++ "FEC Codes" library named "A Fast Forward Error Correction Toolbox" (AFF3CT) library provided via the open source MIT license:

  1) The details of the AFF3CT C++ library including the team that developed the library can be found at:
  https://aff3ct.github.io/

  2) CCSDS rate 1/2 LDPC Decoder (June 2023): A CCSDS rate 1/2 LDPC decoder GNU Radio Out-Of-Tree (OOT) block has been built with the AFF3CT library to demonstrate that a library not originally built and designed for GNU Radio can be incorporated into a GNU Radio module/block (with .so shared library and associated .h header files). Specifically, this repository implements the (2048,1024) rate 1/2 LDPC code in the CCSDS document "TM SYNCHRONIZATION AND CHANNEL CODING", CCSDS 131.0-B-4, Blue Book. The H matrix (Parity Check Matrix) for the code is included in the main directory of this repository (H_matrix_ccsds_ar4ja_r1_2_k1024.alist).

  3) CCSDS Rate 1/2 Encoder (July 2023):  A CCSDS (2048,1024) rate 1/2 LDPC encoder with just the In-Tree GNU Radio FEC blocks is included in the transmit/receive flowgraph in the "examples" folder. The associated Generator matrix (G_matrix_ccsds_ar4ja_r1_2_k1024.alist) for the encoder is included in the main directory of this repository.

  4) DVB-S2 rate 1/2 LDPC Decoder (August 2023): A DVB-S2 rate 1/2 LDPC decoder GNU Radio Out-Of-Tree (OOT) block has also been built with the AFF3CT library. Specifically, this repository implements the (64800,32400) rate 1/2 LDPC code in the ETSI EN 302 307-1 document "Digital Video Broadcasting (DVB);Second generation framing structure, channel coding and modulation systems for Broadcasting, Interactive Services, News Gathering and other broadband satellite applications; Part 1: DVB-S2". The H matrix (Parity Check Matrix) for the code is included in the main directory of this repository (H_matrix_dvbs2_r1_2_k32400.alist). The AFF3CT library code provides the DVB-S2 parity check matrices in "alist" format for user convenience.

  5) DVB-S2 Rate 1/2 Encoder (August 2023):  A DVB-S2 (64800,32400) rate 1/2 LDPC encoder with just the In-Tree GNU Radio FEC blocks is included in the Encode/Decode DVB-S2 flowgraph in the "examples" folder. 
  
  6) This repository is associated with the gr-HighDataRate_Modem repository. gr-aff3ct_codes is being kept separate from the gr-HighDataRate_Modem repository because extra steps/dependencies are required to add the AFF3CT library to a GNU Radio block as listed below after cloning this repository.
  

An example CCSDS SQPSK/LDPC Rate 1/2 flowgraph that plays back and decodes a recorded I/Q file (about 500 frame duration repeated in flowgraph) is located in the “examples” folder of this github repository (SQPSK_LDPC_RateOneHalf_Orion_RCVonly.grc). The SETI Institute used the Allen Telescope Array (ATA) and GNU Radio to record NASA Orion moon vehicle RF IQ on the downlink (SQPSK with CCSDS LDPC rate 1/2) as stated at the following link (following link also provides an additional link to the ATA recorded Orion IQ data files):

            https://www.seti.org/to-the-moon

The other two flowgraphs in the examples folder provides:

     a) CCSDS_BPSK_LDPC_Simulation_TX_RCV.grc provides a simulation BPSK transmit/Receive loop with both the CCSDS rate 1/2 LDPC Decoder and CCSDS rate 1/2 LDPC Encoder. Each test frame for the loop is 1024 bits in length and starts with 4501 Hexadecimal.
     
     b) DVBS2_LDPC_Simulation_Encode_Decode.grc provides a simulation Encode/Decode loop with both the DVB-S2 rate 1/2 LDPC Decoder and DVB-S2 rate 1/2 LDPC Encoder. The test bit stream is a continuous stream of CCSDS 1024 bit frames asynchronously embedded in the DVB-S2 LDPC frames as stated as an option in "CCSDS SPACE LINK PROTOCOLS OVER ETSI DVB-S2 STANDARD", Blue Book, CCSDS 131.3-B-2.

                               INSTALLATION FROM SOURCE

The installation procedure of gr-aff3ct_codes source code includes the usual procedure for a GNU Radio out-of-tree (OOT) module. The detailed instructions are as follows for building from source in a system where GNU Radio 3.10 has already been installed with Ubuntu 22.04:


                                  DOWNLOADING

The gr-aff3ct_codes module and blocks within the module were developed in this DavidToddMiller/gr-aff3ct_codes Github repository. One can use the typical github clone command in the Ubuntu terminal to download all of the needed source code folders and files for example:

“ git clone https://github.com/DavidToddMiller/gr-aff3ct_codes.git ”

                          BUILDING AND INSTALLING

After downloading, gr-aff3ct_codes can be built and installed using cmake. The following can be run inside the directory containing the gr-aff3ct_codes sources:

mkdir build

cd build

sudo cmake ../

sudo make

sudo make install

sudo ldconfig

                               DEPENDENCIES for AFF3CT Shared Library

There are some build dependencies for these GNU Radio out-of-tree modules that must be done before running the above "cmake" and "make" operations that are required to use the AFF3CT Shared Library in a GNU Radio block:

  1) The AFF3CT Library (Header Files and Shared Object File) are already available in this repository, but must be placed in other folder locations before compiling/building so that they are found during build and execution.

               a) aff3ct-3.0.2_Library_Shared_Object_File (see folder): libaff3ct-3.0.2.so Shared Object
                    I placed in root location path and folder usr/lib    
  
               b) aff3ct-3.0.2_Library_Header_Files (see folder)
                    I placed inside ubuntu root location path and folder "usr/include" the following:  
  
                        Folders: date, Factory, Maps, math, Module, Ranges, Splitter, Tools, Types
                    
                        Files: cli.hpp, mipp.h, mipp_impl_AVX.hxx, mipp_impl_AVX512.hxx, mipp_impl_NEON.hxx, mipp_Impl_SSE.hxx, mipp_object.hxx, mipp_scalar_op.h, mipp_scalar_op.hxx, rang.hpp
                    
 
  2) 3 additional changes to cmake already are already in this repository:
 
     a) CMakeLists.txt at top level of repository: Added lines 100-103
 
     b) CMakeLists.txt in "lib" folder:  Modified line 30 at end of line
         
     c) Added new file at location cmake/Modules: Findlibaff3ct-3.0.2.so.cmake 
  
  3) More information on the AFF3CT Library can be found at the following 2 links if one wants to download AFF3CT files and manuals themselves:
 
              
              https://aff3ct.github.io/index.html
              
              https://aff3ct.github.io/download.html
              

                         RUNNING THE .grc FLOWGRAPHS

The .py generated file in the GNU Radio Companion should be run from the Ubuntu terminal when in the gr-aff3ct_codes level of terminal (so that alist files are seen during execution). For example cd gr-aff3ct_codes.


                                         FUTURE WORK   

1. Expand Capabilities by adding other codes in AFF3CT library to GNU Radio blocks. 



                                              
