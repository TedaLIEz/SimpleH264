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

  explicit Slice_header()
      : field_pic_flag(false),
        bottom_field_flag(false),
        idr_pic_id(0),
        pic_order_cnt_lsb(0),
        delta_pic_order_cnt_bottom(0),
        redundant_pic_cnt(0),
        direct_spatial_mv_pred_flag(false),
        num_ref_idx_active_override_flag(false),
        num_ref_idx_l0_active_minus1(0),
        num_ref_idx_l1_active_minus1(0),
        ref_pic_list_reordering_flag_l0(false),
        reordering_of_pic_nums_idc(0),
        abs_diff_pic_num_minus1(0),
        long_term_pic_num(0),
        ref_pic_list_reordering_flag_l1(false),
        luma_log2_weight_denom(0),
        chroma_log2_weight_denom(0),
        luma_weight_l0_flag(false),
        chroma_weight_l0_flag(false),
        luma_weight_l1_flag(false),
        chroma_weight_l1_flag(false),
        no_output_of_prior_pics_flag(false),
        long_term_reference_flag(false),
        adaptive_ref_pic_marking_mode_flag(false),
        memory_management_control_operation(0),
        difference_of_pic_nums_minus1(0),
        long_term_frame_idx(0),
        max_long_term_frame_idx_plus1(0),
        cabac_init_idc(0),
        slice_qp_delta(0),
        sp_for_switch_flag(false),
        slice_qs_delta(0),
        disable_deblocking_filter_idc(0),
        slice_alpha_c0_offset_div2(0),
        slice_beta_offset_div2(0),
        slice_group_change_cycle(0) {}

};

#endif //SIMPLEH264_SL_HEADER_H
