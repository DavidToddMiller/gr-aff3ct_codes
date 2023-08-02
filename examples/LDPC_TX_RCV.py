#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Not titled yet
# GNU Radio version: 3.10.7.0

from packaging.version import Version as StrictVersion
from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio import aff3ct_codes
from gnuradio import analog
from gnuradio import blocks
import pmt
from gnuradio import digital
from gnuradio import fec
from gnuradio import gr
from gnuradio.filter import firdes
from gnuradio.fft import window
import sys
import signal
from PyQt5 import Qt
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
import sip



class LDPC_TX_RCV(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Not titled yet", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Not titled yet")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except BaseException as exc:
            print(f"Qt GUI: Could not set Icon: {str(exc)}", file=sys.stderr)
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "LDPC_TX_RCV")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except BaseException as exc:
            print(f"Qt GUI: Could not restore geometry: {str(exc)}", file=sys.stderr)

        ##################################################
        # Variables
        ##################################################
        self.G = G = fec.ldpc_G_matrix('/home/davem/gr-aff3ct_codes/G_matrix_ccsds_ar4ja_r1_2_k1024.alist')
        self.samp_rate = samp_rate = 2000000
        self.ldpc_enc_G = ldpc_enc_G = fec.ldpc_gen_mtrx_encoder_make(G)
        self.constel = constel = digital.constellation_calcdist(digital.psk_2()[0], digital.psk_2()[1],
        4, 1, digital.constellation.AMPLITUDE_NORMALIZATION).base()
        self.constel.gen_soft_dec_lut(8)

        ##################################################
        # Blocks
        ##################################################

        self.qtgui_const_sink_x_0_0 = qtgui.const_sink_c(
            1024, #size
            'BPSK constellation', #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_const_sink_x_0_0.set_update_time(0.50)
        self.qtgui_const_sink_x_0_0.set_y_axis((-2), 2)
        self.qtgui_const_sink_x_0_0.set_x_axis((-2), 2)
        self.qtgui_const_sink_x_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, "")
        self.qtgui_const_sink_x_0_0.enable_autoscale(False)
        self.qtgui_const_sink_x_0_0.enable_grid(False)
        self.qtgui_const_sink_x_0_0.enable_axis_labels(True)

        self.qtgui_const_sink_x_0_0.disable_legend()

        labels = ['', '', '', '', '',
            '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ["blue", "red", "red", "red", "red",
            "red", "red", "red", "red", "red"]
        styles = [0, 0, 0, 0, 0,
            0, 0, 0, 0, 0]
        markers = [0, 0, 0, 0, 0,
            0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]

        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_const_sink_x_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_const_sink_x_0_0.set_line_label(i, labels[i])
            self.qtgui_const_sink_x_0_0.set_line_width(i, widths[i])
            self.qtgui_const_sink_x_0_0.set_line_color(i, colors[i])
            self.qtgui_const_sink_x_0_0.set_line_style(i, styles[i])
            self.qtgui_const_sink_x_0_0.set_line_marker(i, markers[i])
            self.qtgui_const_sink_x_0_0.set_line_alpha(i, alphas[i])

        self._qtgui_const_sink_x_0_0_win = sip.wrapinstance(self.qtgui_const_sink_x_0_0.qwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_const_sink_x_0_0_win, 1, 0, 1, 1)
        for r in range(1, 2):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.fec_extended_encoder_0_0 = fec.extended_encoder(encoder_obj_list=ldpc_enc_G, threading= None, puncpat='11')
        self.digital_pfb_clock_sync_xxx_0 = digital.pfb_clock_sync_ccf(2, (3.14159/100.0), firdes.root_raised_cosine(32,64,1.0,0.5,1704), 32, 0, 1.5, 1)
        self.digital_costas_loop_cc_0_0 = digital.costas_loop_cc((2*3.14159/1000), 2, False)
        self.digital_constellation_modulator_0_0 = digital.generic_mod(
            constellation=constel,
            differential=False,
            samples_per_symbol=2,
            pre_diff_code=True,
            excess_bw=0.5,
            verbose=False,
            log=False,
            truncate=False)
        self.blocks_unpack_k_bits_bb_0 = blocks.unpack_k_bits_bb(8)
        self.blocks_throttle2_0 = blocks.throttle( gr.sizeof_gr_complex*1, samp_rate, True, 0 if "auto" == "auto" else max( int(float(0.1) * samp_rate) if "auto" == "time" else int(0.1), 1) )
        self.blocks_tag_gate_1 = blocks.tag_gate(gr.sizeof_gr_complex * 1, False)
        self.blocks_tag_gate_1.set_single_key("")
        self.blocks_stream_mux_0 = blocks.stream_mux(gr.sizeof_char*1, (64, 2048))
        self.blocks_stream_demux_0 = blocks.stream_demux(gr.sizeof_float*1, (64,2048))
        self.blocks_pack_k_bits_bb_0 = blocks.pack_k_bits_bb(8)
        self.blocks_null_sink_1 = blocks.null_sink(gr.sizeof_float*1)
        self.blocks_multiply_const_vxx_2_0 = blocks.multiply_const_cc(-1.0)
        self.blocks_multiply_const_vxx_2 = blocks.multiply_const_cc(1.0)
        self.blocks_file_source_0_0_0_0_0_0 = blocks.file_source(gr.sizeof_char*1, '/home/davem/AFF3CT_GithubTest/gr-aff3ct_codes/examples/10_CCSDSframes_Before_LDPC_Encode', True, 0, 0)
        self.blocks_file_source_0_0_0_0_0_0.set_begin_tag(pmt.PMT_NIL)
        self.blocks_file_source_0_0_0_0_0_0.set_processor_affinity([1])
        self.blocks_file_source_0_0_0_0_0 = blocks.file_source(gr.sizeof_char*1, '/home/davem/AFF3CT_GithubTest/gr-aff3ct_codes/examples/64_BIT_ASM_CCSDS_LDPC', True, 0, 0)
        self.blocks_file_source_0_0_0_0_0.set_begin_tag(pmt.PMT_NIL)
        self.blocks_file_source_0_0_0_0_0.set_processor_affinity([1])
        self.blocks_file_sink_0_0 = blocks.file_sink(gr.sizeof_char*1, '/home/davem/AFF3CT_GithubTest/gr-aff3ct_codes/examples/CCSDS_LDPC_SINK', False)
        self.blocks_file_sink_0_0.set_unbuffered(False)
        self.blocks_complex_to_real_0 = blocks.complex_to_real(1)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, (2.75/4), 0)
        self.analog_const_source_x_0 = analog.sig_source_f(0, analog.GR_CONST_WAVE, 0, 0, 1)
        self.aff3ct_codes_ccsds_ldpc_decoder_0 = aff3ct_codes.ccsds_ldpc_decoder(1, True, 40)
        self.aff3ct_codes_Tag_FrameASM64_ff_0 = aff3ct_codes.Tag_FrameASM64_ff(1, 64, 2)
        self.aff3ct_codes_Resolve_Phase2_0 = aff3ct_codes.Resolve_Phase2(1, 2112, 30000, 3)
        self.aff3ct_codes_Frame_ExtractOQPSK_0 = aff3ct_codes.Frame_ExtractOQPSK(2112, 30000, 64)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.aff3ct_codes_Frame_ExtractOQPSK_0, 0), (self.aff3ct_codes_Resolve_Phase2_0, 0))
        self.connect((self.aff3ct_codes_Resolve_Phase2_0, 0), (self.blocks_stream_demux_0, 0))
        self.connect((self.aff3ct_codes_Tag_FrameASM64_ff_0, 0), (self.aff3ct_codes_Frame_ExtractOQPSK_0, 0))
        self.connect((self.aff3ct_codes_ccsds_ldpc_decoder_0, 0), (self.blocks_file_sink_0_0, 0))
        self.connect((self.analog_const_source_x_0, 0), (self.aff3ct_codes_Frame_ExtractOQPSK_0, 1))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_throttle2_0, 0))
        self.connect((self.blocks_complex_to_real_0, 0), (self.aff3ct_codes_Tag_FrameASM64_ff_0, 0))
        self.connect((self.blocks_file_source_0_0_0_0_0, 0), (self.blocks_stream_mux_0, 0))
        self.connect((self.blocks_file_source_0_0_0_0_0_0, 0), (self.blocks_unpack_k_bits_bb_0, 0))
        self.connect((self.blocks_multiply_const_vxx_2, 0), (self.digital_pfb_clock_sync_xxx_0, 0))
        self.connect((self.blocks_multiply_const_vxx_2_0, 0), (self.blocks_complex_to_real_0, 0))
        self.connect((self.blocks_pack_k_bits_bb_0, 0), (self.digital_constellation_modulator_0_0, 0))
        self.connect((self.blocks_stream_demux_0, 1), (self.aff3ct_codes_ccsds_ldpc_decoder_0, 0))
        self.connect((self.blocks_stream_demux_0, 0), (self.blocks_null_sink_1, 0))
        self.connect((self.blocks_stream_mux_0, 0), (self.blocks_pack_k_bits_bb_0, 0))
        self.connect((self.blocks_tag_gate_1, 0), (self.blocks_multiply_const_vxx_2_0, 0))
        self.connect((self.blocks_throttle2_0, 0), (self.blocks_multiply_const_vxx_2, 0))
        self.connect((self.blocks_unpack_k_bits_bb_0, 0), (self.fec_extended_encoder_0_0, 0))
        self.connect((self.digital_constellation_modulator_0_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.digital_costas_loop_cc_0_0, 0), (self.blocks_tag_gate_1, 0))
        self.connect((self.digital_costas_loop_cc_0_0, 0), (self.qtgui_const_sink_x_0_0, 0))
        self.connect((self.digital_pfb_clock_sync_xxx_0, 0), (self.digital_costas_loop_cc_0_0, 0))
        self.connect((self.fec_extended_encoder_0_0, 0), (self.blocks_stream_mux_0, 1))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "LDPC_TX_RCV")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_G(self):
        return self.G

    def set_G(self, G):
        self.G = G

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle2_0.set_sample_rate(self.samp_rate)

    def get_ldpc_enc_G(self):
        return self.ldpc_enc_G

    def set_ldpc_enc_G(self, ldpc_enc_G):
        self.ldpc_enc_G = ldpc_enc_G

    def get_constel(self):
        return self.constel

    def set_constel(self, constel):
        self.constel = constel




def main(top_block_cls=LDPC_TX_RCV, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    qapp.exec_()

if __name__ == '__main__':
    main()
