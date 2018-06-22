#! /bin/sh
#
# run_ITRI_tiny_yolov1.sh
# Copyright (C) 2018 lucas <lucas@lucas>
#
# Distributed under terms of the MIT license.
#

python check_data_file.py cd_out_weight_files cd_out_qweight_files cd_out_input_files cd_out_qinput_files cd_out_out_files cd_out_qout_files prediction_cube.bin golden_prediction_cube.bin

