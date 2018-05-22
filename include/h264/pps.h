//
// Created by aLIEzTed on 5/21/18.
// structure according to Picture parameter set RBSP syntax
// RBSP: Raw Byte Sequence Payload
//

#ifndef SIMPLEH264_PPS_H
#define SIMPLEH264_PPS_H
#include <vector>
class PPS {
 public:
  int pic_parameter_set_id;
  int seq_parameter_set_id;
  bool entropy_coding_mode_flag;
  bool bottom_field_pic_order_in_frame_present_flag;
  int num_slice_groups_minus1;

  int slice_group_map_type;  //01101000

  std::vector<int> run_length_minus1;

  std::vector<int> top_left;
  std::vector<int> bottom_right;

  bool slice_group_change_direction_flag;
  int slice_group_change_rate_minus1;

  int pic_size_in_map_units_minus1;
  std::vector<int> slice_group_id;

  int num_ref_idx_l0_default_active_minus1;
  int num_ref_idx_l1_default_active_minus1;
  bool weighted_pred_flag;
  int weighted_bipred_idc;
  int pic_init_qp_minus26;
  int pic_init_qs_minus26;
  int chroma_qp_index_offset;
  bool deblocking_filter_control_present_flag;
  bool constrained_intra_pred_flag;
  bool redundant_pic_cnt_present_flag;

  bool transform_8x8_mode_flag;

  bool pic_scaling_matrix_present_flag;
  std::vector<bool> pic_scaling_list_present_flag;

  int second_chroma_qp_index_offset;

  explicit PPS()
      : slice_group_map_type(0),
        slice_group_change_direction_flag(false),
        slice_group_change_rate_minus1(0),
        pic_size_in_map_units_minus1(0),
        transform_8x8_mode_flag(false),
        pic_scaling_matrix_present_flag(false),
        second_chroma_qp_index_offset(0) {}

};
#endif //SIMPLEH264_PPS_H
