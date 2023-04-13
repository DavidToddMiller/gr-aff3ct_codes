                                  gr-aff3ct_codes

gr-aff3ct_codes is a GNU Radio module set of blocks that incorporates codes from the Massachusetts Institute of Technology (MIT)"A Fast Forward Error Correction Toolbox" (AFF3CT) library:
 
  1) Current Repository Status (April 2023): Initially, a CCSDS rate 1/2 LDPC decoder GNU Radio block has been built with the MIT AFF3CT library to demonstrate that a library not originally built and designed for GNU Radio can be incorporated into a GNU Radio module/block (with .so shared library and associated .h header files).

  2) Future: Add additional codes from MIT AFF3CT library into GNU Radio OOT decoder and encoder blocks.
  
  3) This repository is associated with the gr-HighDataRate_Modem repository. gr-aff3ct_codes is being kept separate from the gr-HighDataRate_Modem repository because extra steps/dependencies are required to add the MIT AFF3CT library to a GNU Radio block as listed below after cloning this repository.
  

Also, an example SQPSK/LDPC flowgraph that plays back a recorded I/Q file (about 500 frame duration) is located in the “examples” folder of this github repository. The Allen Antenna Array was used to record Orion moon vehicle IQ on the downlink (SQPSK with CCSDS LDPC) as stated at the following link that also provides recorded IQ data files and all the details about the SQPSK/LDPC flowgraph also:

            https://destevez.net/2022/11/decoding-the-artemis-i-orion-vehicle/


                               INSTALLATION FROM SOURCE

The installation procedure of gr-aff3ct_codes source code includes the usual procedure for a GNU Radio out-of-tree (OOT) module. The detailed instructions are as follows for building from source in a system where GNU Radio 3.10 (should work with GNU Radio version 3.9 also) has already been installed with Ubuntu 22.04:


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

There are some build dependencies for this GNU Radio out-of-tree module that must be done before running the above "cmake" and "make" operations that are required to use the MIT AFF3CT Shared Library in a GNU Radio block:

  1) The MIT AFF3CT Library (Header Files and Shared Object File) are already available in this repository, but must be placed in other folder locations before compiling/building so that they are found during build and execution.

  
                a) aff3ct-3.0.2_Library_Header_Files (see folder)
                    I placed in root location path and folder usr/include but other options exist
                     
                b) aff3ct-3.0.2_Library_Shared_Object_File (see folder): libaff3ct-3.0.2.so Shared Object
                    I placed in root location path and folder usr/lib but other options exist   

  2) 3 additional changes to cmake already are already in this repository:
 
     a) CMakeLists.txt at top level of repository: Added lines 100-103
 
     b) CMakeLists.txt in "lib" folder:  Modified line 30 at end of line
         
     c) Added new file at location cmake/Modules: Findlibaff3ct-3.0.2.so.cmake 
  
  3) More information on the MIT AFF3CT Library can be found at the following 2 links if one wants to download AFF3CT files and manuals themselves:
 
              
              https://aff3ct.github.io/index.html
              
              https://aff3ct.github.io/download.html
              

                         RUNNING THE .grc FLOWGRAPHS

The .py generated file in the GNU Radio Companion should be run from the Ubuntu terminal when in the gr-aff3ct_codes level of terminal (so alist file is seen during execution). For example cd gr-aff3ct_codes)


                                         FUTURE WORK   

1. Expand Capabilities by adding other codes in AA3CT library to GNU Radio blocks. 
2. Add Encoders and and additional Decoders (emphasizing CCSDS codes)  



                                              
