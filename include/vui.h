//
// Created by aLIEzTed on 5/19/18.
//

#ifndef SIMPLEH264_VUI_H
#define SIMPLEH264_VUI_H
#include "hrd.h"
class VUI {
 public:
  bool aspect_ratio_info_present_flag;
  unsigned int aspect_ratio_idc;  // 0 - 255 as 255 is Extended_SAR
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
  HRD nal_hrd_param;

  bool vcl_hrd_parameters_present_flag;

  // TODO: HRD Param
  HRD vcl_hrd_param;

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

  explicit VUI()
      : aspect_ratio_idc(0),
        sar_width(0),
        sar_height(0),
        overscan_appropriate_flag(false),
        video_format(0),
        video_full_range_flag(false),
        colour_description_present_flag(false),
        colour_primaries(0),
        transfer_characteristics(0),
        matrix_coefficients(0),
        chroma_sample_loc_type_top_field(0),
        chroma_sample_loc_type_bottom_field(0),
        num_units_in_tick(0),
        time_scale(0),
        fixed_frame_rate_flag(false),
        low_delay_hrd_flag(false),
        motion_vectors_over_pic_boundaries_flag(false),
        max_bytes_per_pic_denom(0),
        max_bits_per_mb_denom(0),
        log2_max_mv_length_horizontal(0),
        log2_max_mv_length_vertical(0),
        max_num_reorder_frames(0),
        max_dec_frame_buffering(0) {

  }
};
#endif //SIMPLEH264_VUI_H
