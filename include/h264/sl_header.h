//
// Created by aLIEzTed on 5/29/18.
// Define structure of slice header
//

#ifndef SIMPLEH264_SL_HEADER_H
#define SIMPLEH264_SL_HEADER_H
#include <vector>
typedef std::vector<std::vector<int>> grid;
class Slice_header {
 public:
  int first_mb_in_slice;
  int slice_type;
  int pic_parameter_set_id;
  int frame_num;

  bool field_pic_flag;
  bool bottom_field_flag;

  int idr_pic_id;

  int pic_order_cnt_lsb;

  int delta_pic_order_cnt_bottom;

  int delta_pic_order_cnt[2];

  int redundant_pic_cnt;

  bool direct_spatial_mv_pred_flag;

  bool num_ref_idx_active_override_flag;

  int num_ref_idx_l0_active_minus1;

  int num_ref_idx_l1_active_minus1;

  // ref_pic_list_reordering
  bool ref_pic_list_reordering_flag_l0;
  int reordering_of_pic_nums_idc;
  int abs_diff_pic_num_minus1;
  int long_term_pic_num;   // also used in dec_ref_pic_marking
  bool ref_pic_list_reordering_flag_l1;
  // ref_pic_list_reordering end


  // pred_weight_table
  int luma_log2_weight_denom;
  int chroma_log2_weight_denom;
  bool luma_weight_l0_flag;
  std::vector<int> luma_weight_l0;
  std::vector<int> luma_offset_l0;
  bool chroma_weight_l0_flag;
  grid chroma_weight_l0;
  grid chroma_offset_l0;

  bool luma_weight_l1_flag;
  std::vector<int> luma_weight_l1;
  std::vector<int> luma_offset_l1;

  bool chroma_weight_l1_flag;
  grid chroma_weight_l1;
  grid chroma_offset_l1;
  // pred_weight_table end


  // dec_ref_pic_marking
  bool no_output_of_prior_pics_flag;
  bool long_term_reference_flag;
  bool adaptive_ref_pic_marking_mode_flag;
  int memory_management_control_operation;
  int difference_of_pic_nums_minus1;
  // long_term_pic_num here, which is defined above
  int long_term_frame_idx;
  int max_long_term_frame_idx_plus1;

  // dec_ref_pic_marking end



  int cabac_init_idc;

  int slice_qp_delta;

  bool sp_for_switch_flag;

  int slice_qs_delta;


  int disable_deblocking_filter_idc;

  int slice_alpha_c0_offset_div2;
  int slice_beta_offset_div2;

  int slice_group_change_cycle;

};

#endif //SIMPLEH264_SL_HEADER_H
