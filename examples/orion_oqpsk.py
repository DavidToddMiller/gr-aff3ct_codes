#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Orion OQPSK demodulator
# Author: Daniel Estevez
# GNU Radio version: 3.10.5.1

from packaging.version import Version as StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print("Warning: failed to XInitThreads()")

from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio.filter import firdes
import sip
from gnuradio import aff3ct_codes
from gnuradio import blocks
import pmt
from gnuradio import digital
from gnuradio import filter
from gnuradio import gr
from gnuradio.fft import window
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
import numpy as np



from gnuradio import qtgui

class orion_oqpsk(gr.top_block, Qt.QWidget):

    def __init__(self, basename='/home/daniel/Artemis1/Orion_2022-11-16T16:22:25.503183_0', doppler=(-19e3), phase_difference=(-14.3), power_ratio=0.45):
        gr.top_block.__init__(self, "Orion OQPSK demodulator", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Orion OQPSK demodulator")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
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

        self.settings = Qt.QSettings("GNU Radio", "orion_oqpsk")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        ##################################################
        # Parameters
        ##################################################
        self.basename = basename
        self.doppler = doppler
        self.phase_difference = phase_difference
        self.power_ratio = power_ratio

        ##################################################
        # Variables
        ##################################################
        self.syncword_hex = syncword_hex = '034776C7272895B0'
        self.syncword = syncword = 2*np.unpackbits(np.frombuffer(bytes.fromhex(syncword_hex), dtype = 'uint8')).astype('int')-1
        self.samp_rate = samp_rate = 1024000*2
        self.baudrate = baudrate = 2000000
        self.sync_threshold = sync_threshold = 8
        self.sps = sps = samp_rate/baudrate
        self.access_code = access_code = ''.join([str(i) for i in (syncword+1)//2])

        ##################################################
        # Blocks
        ##################################################

        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
            128, #size
            samp_rate, #samp_rate
            "", #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0.set_update_time(0.5)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0.enable_tags(True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0.enable_stem_plot(False)


        labels = ['Signal 1', 'Signal 2', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.qwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_win)
        self.qtgui_const_sink_x_0 = qtgui.const_sink_c(
            512, #size
            'OQPSK constellation', #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_const_sink_x_0.set_update_time(0.30)
        self.qtgui_const_sink_x_0.set_y_axis((-2), 2)
        self.qtgui_const_sink_x_0.set_x_axis((-2), 2)
        self.qtgui_const_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, "")
        self.qtgui_const_sink_x_0.enable_autoscale(False)
        self.qtgui_const_sink_x_0.enable_grid(False)
        self.qtgui_const_sink_x_0.enable_axis_labels(True)


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
        alphas = [1, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]

        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_const_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_const_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_const_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_const_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_const_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_const_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_const_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_const_sink_x_0_win = sip.wrapinstance(self.qtgui_const_sink_x_0.qwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_const_sink_x_0_win, 0, 1, 1, 1)
        for r in range(0, 1):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(1, 2):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.fir_filter_xxx_0_0 = filter.fir_filter_ccc(1, [1/4]*4)
        self.fir_filter_xxx_0_0.declare_sample_delay(0)
        self.digital_symbol_sync_xx_0_0_0 = digital.symbol_sync_cc(
            digital.TED_GARDNER,
            4,
            (1e-3),
            1.0,
            1.0,
            0.001,
            1,
            digital.constellation_bpsk().base(),
            digital.IR_MMSE_8TAP,
            128,
            [])
        self.digital_costas_loop_cc_0_0_0 = digital.costas_loop_cc((1e-4), 4, False)
        self.digital_binary_slicer_fb_0_0 = digital.binary_slicer_fb()
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_tag_gate_0 = blocks.tag_gate(gr.sizeof_float * 1, False)
        self.blocks_tag_gate_0.set_single_key("")
        self.blocks_stream_demux_0 = blocks.stream_demux(gr.sizeof_float*1, (64, 2048))
        self.blocks_pack_k_bits_bb_0_0 = blocks.pack_k_bits_bb(8)
        self.blocks_pack_k_bits_bb_0 = blocks.pack_k_bits_bb(8)
        self.blocks_null_sink_1 = blocks.null_sink(gr.sizeof_float*1)
        self.blocks_null_sink_0_1 = blocks.null_sink(gr.sizeof_float*1)
        self.blocks_null_sink_0_0 = blocks.null_sink(gr.sizeof_float*1)
        self.blocks_multiply_const_vxx_1 = blocks.multiply_const_ff((-1))
        self.blocks_interleave_0_0 = blocks.interleave(gr.sizeof_float*1, 1)
        self.blocks_interleave_0 = blocks.interleave(gr.sizeof_float*1, 1)
        self.blocks_float_to_complex_0_0 = blocks.float_to_complex(1)
        self.blocks_file_source_0_0 = blocks.file_source(gr.sizeof_gr_complex*1, '/home/davem/gr-aff3ct_codes/examples/Orion_LDPC_RF_IQ_Recording_1minute_Only', True, 0, 0)
        self.blocks_file_source_0_0.set_begin_tag(pmt.PMT_NIL)
        self.blocks_file_sink_1 = blocks.file_sink(gr.sizeof_char*1, '/home/davem/gr-aff3ct_codes/examples/Before_LDPC_Decode_Sink', False)
        self.blocks_file_sink_1.set_unbuffered(False)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*1, '/home/davem/gr-aff3ct_codes/examples/Test_LDPC_Sink', False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.blocks_delay_0_0 = blocks.delay(gr.sizeof_float*1, 2)
        self.blocks_delay_0 = blocks.delay(gr.sizeof_float*1, 1)
        self.blocks_complex_to_float_0_0 = blocks.complex_to_float(1)
        self.blocks_complex_to_float_0 = blocks.complex_to_float(1)
        self.aff3ct_codes_ccsds_ldpc_decoder_0 = aff3ct_codes.ccsds_ldpc_decoder(1, True, 40)
        self.aff3ct_codes_Tag_FrameASM64_ff_0 = aff3ct_codes.Tag_FrameASM64_ff(1, 64, 1)
        self.aff3ct_codes_Resolve_Phase2_0 = aff3ct_codes.Resolve_Phase2(1, 2112, 30000, 7)
        self.aff3ct_codes_Frame_Extract2_0 = aff3ct_codes.Frame_Extract2(2112, 30000, 64)
        self.aff3ct_codes_CCSDS_Pseudo_Randomizer_0 = aff3ct_codes.CCSDS_Pseudo_Randomizer(7, 2048, 1)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.aff3ct_codes_CCSDS_Pseudo_Randomizer_0, 0), (self.aff3ct_codes_ccsds_ldpc_decoder_0, 0))
        self.connect((self.aff3ct_codes_CCSDS_Pseudo_Randomizer_0, 0), (self.digital_binary_slicer_fb_0_0, 0))
        self.connect((self.aff3ct_codes_Frame_Extract2_0, 0), (self.aff3ct_codes_Resolve_Phase2_0, 0))
        self.connect((self.aff3ct_codes_Resolve_Phase2_0, 0), (self.blocks_stream_demux_0, 0))
        self.connect((self.aff3ct_codes_Tag_FrameASM64_ff_0, 0), (self.aff3ct_codes_Frame_Extract2_0, 0))
        self.connect((self.aff3ct_codes_ccsds_ldpc_decoder_0, 0), (self.blocks_pack_k_bits_bb_0, 0))
        self.connect((self.blocks_complex_to_float_0, 1), (self.blocks_delay_0, 0))
        self.connect((self.blocks_complex_to_float_0, 1), (self.blocks_interleave_0, 1))
        self.connect((self.blocks_complex_to_float_0, 0), (self.blocks_interleave_0, 0))
        self.connect((self.blocks_complex_to_float_0, 0), (self.blocks_multiply_const_vxx_1, 0))
        self.connect((self.blocks_complex_to_float_0_0, 0), (self.blocks_delay_0_0, 0))
        self.connect((self.blocks_complex_to_float_0_0, 1), (self.blocks_float_to_complex_0_0, 1))
        self.connect((self.blocks_delay_0, 0), (self.blocks_interleave_0_0, 0))
        self.connect((self.blocks_delay_0_0, 0), (self.blocks_float_to_complex_0_0, 0))
        self.connect((self.blocks_file_source_0_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_float_to_complex_0_0, 0), (self.fir_filter_xxx_0_0, 0))
        self.connect((self.blocks_interleave_0, 0), (self.blocks_null_sink_0_1, 0))
        self.connect((self.blocks_interleave_0_0, 0), (self.blocks_null_sink_0_0, 0))
        self.connect((self.blocks_interleave_0_0, 0), (self.blocks_tag_gate_0, 0))
        self.connect((self.blocks_interleave_0_0, 0), (self.qtgui_time_sink_x_0, 0))
        self.connect((self.blocks_multiply_const_vxx_1, 0), (self.blocks_interleave_0_0, 1))
        self.connect((self.blocks_pack_k_bits_bb_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.blocks_pack_k_bits_bb_0_0, 0), (self.blocks_file_sink_1, 0))
        self.connect((self.blocks_stream_demux_0, 1), (self.aff3ct_codes_CCSDS_Pseudo_Randomizer_0, 0))
        self.connect((self.blocks_stream_demux_0, 0), (self.blocks_null_sink_1, 0))
        self.connect((self.blocks_tag_gate_0, 0), (self.aff3ct_codes_Tag_FrameASM64_ff_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.digital_costas_loop_cc_0_0_0, 0))
        self.connect((self.digital_binary_slicer_fb_0_0, 0), (self.blocks_pack_k_bits_bb_0_0, 0))
        self.connect((self.digital_costas_loop_cc_0_0_0, 0), (self.blocks_complex_to_float_0_0, 0))
        self.connect((self.digital_symbol_sync_xx_0_0_0, 0), (self.blocks_complex_to_float_0, 0))
        self.connect((self.digital_symbol_sync_xx_0_0_0, 0), (self.qtgui_const_sink_x_0, 0))
        self.connect((self.fir_filter_xxx_0_0, 0), (self.digital_symbol_sync_xx_0_0_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "orion_oqpsk")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_basename(self):
        return self.basename

    def set_basename(self, basename):
        self.basename = basename

    def get_doppler(self):
        return self.doppler

    def set_doppler(self, doppler):
        self.doppler = doppler

    def get_phase_difference(self):
        return self.phase_difference

    def set_phase_difference(self, phase_difference):
        self.phase_difference = phase_difference

    def get_power_ratio(self):
        return self.power_ratio

    def set_power_ratio(self, power_ratio):
        self.power_ratio = power_ratio

    def get_syncword_hex(self):
        return self.syncword_hex

    def set_syncword_hex(self, syncword_hex):
        self.syncword_hex = syncword_hex
        self.set_syncword(2*np.unpackbits(np.frombuffer(bytes.fromhex(self.syncword_hex), dtype = 'uint8')).astype('int')-1)

    def get_syncword(self):
        return self.syncword

    def set_syncword(self, syncword):
        self.syncword = syncword
        self.set_access_code(''.join([str(i) for i in (self.syncword+1)//2]))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_sps(self.samp_rate/self.baudrate)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate)

    def get_baudrate(self):
        return self.baudrate

    def set_baudrate(self, baudrate):
        self.baudrate = baudrate
        self.set_sps(self.samp_rate/self.baudrate)

    def get_sync_threshold(self):
        return self.sync_threshold

    def set_sync_threshold(self, sync_threshold):
        self.sync_threshold = sync_threshold

    def get_sps(self):
        return self.sps

    def set_sps(self, sps):
        self.sps = sps

    def get_access_code(self):
        return self.access_code

    def set_access_code(self, access_code):
        self.access_code = access_code



def argument_parser():
    parser = ArgumentParser()
    parser.add_argument(
        "--basename", dest="basename", type=str, default='/home/daniel/Artemis1/Orion_2022-11-16T16:22:25.503183_0',
        help="Set Base name for the input/output files (includes the antenna but not the polarization) [default=%(default)r]")
    parser.add_argument(
        "--doppler", dest="doppler", type=eng_float, default=eng_notation.num_to_str(float((-19e3))),
        help="Set Doppler (Hz) [default=%(default)r]")
    parser.add_argument(
        "--phase-difference", dest="phase_difference", type=eng_float, default=eng_notation.num_to_str(float((-14.3))),
        help="Set phase_difference [default=%(default)r]")
    parser.add_argument(
        "--power-ratio", dest="power_ratio", type=eng_float, default=eng_notation.num_to_str(float(0.45)),
        help="Set power_ratio [default=%(default)r]")
    return parser


def main(top_block_cls=orion_oqpsk, options=None):
    if options is None:
        options = argument_parser().parse_args()

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls(basename=options.basename, doppler=options.doppler, phase_difference=options.phase_difference, power_ratio=options.power_ratio)

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
