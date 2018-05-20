//
// Created by aLIEzTed on 5/19/18.
//

#ifndef SIMPLEH264_VUI_H
#define SIMPLEH264_VUI_H
class VUI {
 public:
  bool aspect_ratio_info_present_flag;
  int aspect_ratio_idc;  // 0 - 255 as 255 is Extended_SAR
  int sar_width;
  int sar_height;
  bool overscan_info_present_flag;
  bool overscan_appropriate_flag;

  bool video_signal_type_present_flag;
  int video_format;
  bool video_full_range_flag;
  bool colour_description_present_flag;

  int colour_primaries;
  int transfer_characteristics;
  int matrix_coefficients;

  bool chroma_loc_info_present_flag;

  int chroma_sample_loc_type_top_field;
  int chroma_sample_loc_type_bottom_field;

  bool timing_info_present_flag;

  int num_units_in_tick;
  int time_scale;
  bool fixed_frame_rate_flag;

  bool nal_hrd_parameters_present_flag;

  // TODO: HRD Param

  bool vcl_hrd_parameters_present_flag;

  // TODO: HRD Param

  bool low_delay_hrd_flag;

  bool pic_struct_present_flag;
  bool bitstream_restriction_flag;
  bool motion_vectors_over_pic_boundaries_flag;
  int max_bytes_per_pic_denom;
  int max_bits_per_mb_denom;
  int log2_max_mv_length_horizontal;
  int log2_max_mv_length_vertical;
  int max_num_reorder_frames;
  int max_dec_frame_buffering;

};
#endif //SIMPLEH264_VUI_H
