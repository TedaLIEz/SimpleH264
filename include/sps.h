//
// Created by aLIEzTed on 5/19/18.
//

#ifndef SIMPLEH264_SPS_H
#define SIMPLEH264_SPS_H
#include "debug.h"
#include "vui.h"
#include <vector>
class Sps {
 public:
  int profile_idc = -1;
  bool cons_set0_flag;
  bool cons_set1_flag;
  bool cons_set2_flag;
  bool cons_set3_flag;
  bool cons_set4_flag;
  bool cons_set5_flag;
  int level_idc = -1;
  int seq_parameter_set_id = -1;
  int chroma_format_idc = -1;
  int separate_colour_plane_flag = -1;
  int bit_depth_luma_minus8 = -1;
  int bit_depth_chroma_minus8 = -1;
  bool qpprime_y_zero_transform_bypass_flag;
  bool seq_scaling_matrix_present_flag;
  std::vector<bool> seq_scaling_list_present_flag;
  int scaling_list_16[16]{};
  int scaling_list_64[64]{};
  bool use_default_scaling_matrix_flag_16[16]{};
  bool use_default_scaling_matrix_flag_64[64]{};
  int log2_max_frame_num_minus4;
  int pic_order_cnt_type;

  int log2_max_pic_order_cnt_lsb_minus4 = -1;

  bool delta_pic_order_always_zero_flag = false;
  int offset_for_non_ref_pic = -1;
  int offset_for_top_to_bottom_field = -1;
  int num_ref_frames_in_pic_order_cnt_cycle = -1;
  std::vector<int> offset_for_ref_frame;

  int max_num_ref_frames;
  bool gaps_in_frame_num_value_allowed_flag;
  int pic_width_in_mbs_minus1;
  int pic_height_in_map_units_minus1;
  bool frame_mbs_only_flag;

  bool mb_adaptive_frame_field_flag;

  bool direct_8x8_inference_flag;
  bool frame_cropping_flag;

  int frame_crop_left_offset;
  int frame_crop_right_offset;
  int frame_crop_top_offset;
  int frame_crop_bottom_offset;
  bool vui_parameters_present_flag;
  VUI vui;
};
#endif //SIMPLEH264_SPS_H
