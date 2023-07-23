                                  gr-aff3ct_codes

gr-aff3ct_codes is a GNU Radio module set of blocks that incorporates codes from the Massachusetts Institute of Technology (MIT)"A Fast Forward Error Correction Toolbox" (AFF3CT) library:
 
  1) CCSDS rate 1/2 LDPC Decoder: Current Repository Status (June 2023): A CCSDS rate 1/2 LDPC decoder GNU Radio Out-Of-Tree (OOT) block has been built with the MIT AFF3CT library to demonstrate that a library not originally built and designed for GNU Radio can be incorporated into a GNU Radio module/block (with .so shared library and associated .h header files). Specifically, this repository implements the (2048,1024) rate 1/2 LDPC code in the CCSDS document "TM SYNCHRONIZATION AND CHANNEL CODING", CCSDS 131.0-B-4, Blue Book. The H matrix (Parity Check MAtrix) for the code is included in the main directory of this repository (H_matrix_ccsds_ar4ja_r1_2_k1024.alist).

  2) Encoder:  A CCSDS (2048,1024) rate 1/2 LDPC encoder with just the In-Tree GNU Radio FEC blocks is included in the transmit/receive flowgraph in the "examples" folder. The associated Generator matrix (G_matrix_ccsds_ar4ja_r1_2_k1024.alist) for the encoder is included in the main directory of this repository.
  
  3) This repository is associated with the gr-HighDataRate_Modem repository. gr-aff3ct_codes is being kept separate from the gr-HighDataRate_Modem repository because extra steps/dependencies are required to add the MIT AFF3CT library to a GNU Radio block as listed below after cloning this repository.
  

An example CCSDS SQPSK/LDPC Rate 1/2 flowgraph that plays back and decodes a recorded I/Q file (about 500 frame duration repeated in flowgraph) is located in the “examples” folder of this github repository (SQPSK_LDPC_RateOneHalf_Orion_RCVonly.grc). The SETI Institute used the Allen Telescope Array (ATA) and GNU Radio to record NASA Orion moon vehicle RF IQ on the downlink (SQPSK with CCSDS LDPC rate 1/2) as stated at the following link (following link also provides an additional link to the ATA recorded Orion IQ data files):

            https://www.seti.org/to-the-moon

The other flowgraph in the examples folder (CCSDS_BPSK_LDPC_Simulation_TX_RCV.grc) provides a simulation BPSK transmit/Receive loop with both the CCSDS rate 1/2 LDPC Decoder and CCSDS rate 1/2 LDPC Encoder. Each test frame for the loop is 1024 bits in length and starts with 4501 Hexadecimal.

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

                               DEPENDENCIES for MIT AFF3CT Shared Library

There are some build dependencies for these GNU Radio out-of-tree modules that must be done before running the above "cmake" and "make" operations that are required to use the MIT AFF3CT Shared Library in a GNU Radio block:

  1) The MIT AFF3CT Library (Header Files and Shared Object File) are already available in this repository, but must be placed in other folder locations before compiling/building so that they are found during build and execution.

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
  
  3) More information on the MIT AFF3CT Library can be found at the following 2 links if one wants to download AFF3CT files and manuals themselves:
 
              
              https://aff3ct.github.io/index.html
              
              https://aff3ct.github.io/download.html
              

                         RUNNING THE .grc FLOWGRAPHS

The .py generated file in the GNU Radio Companion should be run from the Ubuntu terminal when in the gr-aff3ct_codes level of terminal (so that alist files are seen during execution). For example cd gr-aff3ct_codes)


                                         FUTURE WORK   

1. Expand Capabilities by adding other codes in AFF3CT library to GNU Radio blocks. 



                                              
