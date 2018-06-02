//
// Created by aLIEzTed on 5/31/18.
//

#include <parser/slice_parser.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class SliceHeaderTestCase : public ::testing::Test {
 protected:

  static Slice_Parser* slice_parser;
  static void SetUpTestCase() {
    slice_parser = new Slice_Parser();
    SPS mock_sps{};
    mock_sps.seq_parameter_set_id = 0;
    mock_sps.frame_mbs_only_flag = true;
    mock_sps.pic_order_cnt_type = 0;
    mock_sps.delta_pic_order_always_zero_flag = false;
    mock_sps.log2_max_frame_num_minus4 = 1;
    mock_sps.log2_max_pic_order_cnt_lsb_minus4 = 3;
    PPS mock_pps{};
    mock_pps.seq_parameter_set_id = 0;
    mock_pps.pic_parameter_set_id = 0;
    mock_pps.pic_order_present_flag = true;
    mock_pps.redundant_pic_cnt_present_flag = false;
    mock_pps.num_ref_idx_l0_default_active_minus1 = 0;
    mock_pps.num_ref_idx_l1_default_active_minus1 = 0;
    mock_pps.weighted_pred_flag = false;
    mock_pps.weighted_bipred_idc = 0;
    mock_pps.entropy_coding_mode_flag = false;
    mock_pps.deblocking_filter_variables_present_flag = true;
    mock_pps.num_slice_groups_minus1 = 0;
    mock_pps.slice_group_map_type = 0;
    auto ctx = Context::getInstance();
    ctx->add_pps(mock_pps);
    ctx->add_sps(mock_sps);


  }
};

Slice_Parser* SliceHeaderTestCase::slice_parser;

TEST_F(SliceHeaderTestCase, Parse_Header_Test) {
  unsigned char data[33] = {0x25, 0xb8, 0x20, 0x21, 0x7f, 0xff, 0xf3,
                            0xc5, 0x00, 0x01, 0x01, 0xdf, 0xea, 0xbc,
                            0x71, 0x8e, 0x19, 0xf2, 0x62, 0x3b, 0x3d,
                            0x34, 0xf2, 0x5d, 0xd0, 0xfb, 0x23, 0xe8,
                            0x1b, 0xcf, 0x9c, 0x83, 0x79};
  unsigned long offset = 0;
  auto header = slice_parser->parse_header(data, offset);
  EXPECT_EQ(header.first_mb_in_slice, 0);
  EXPECT_EQ(header.slice_type, SLICE_TYPE_I);
  EXPECT_EQ(header.pic_parameter_set_id, 0);
  EXPECT_EQ(header.frame_num, 0);
  EXPECT_EQ(header.field_pic_flag, false);
  EXPECT_EQ(header.bottom_field_flag, false);
  EXPECT_EQ(header.idr_pic_id, 0);
  EXPECT_EQ(header.pic_order_cnt_lsb, 0);
  EXPECT_EQ(header.delta_pic_order_cnt_bottom, 0);
  EXPECT_EQ(header.redundant_pic_cnt, 0);
  EXPECT_EQ(header.direct_spatial_mv_pred_flag, false);
  EXPECT_EQ(header.num_ref_idx_active_override_flag, false);
  EXPECT_EQ(header.num_ref_idx_l0_active_minus1, 0);
  EXPECT_EQ(header.num_ref_idx_l1_active_minus1, 0);

  // ref pic list reordering
  EXPECT_EQ(header.ref_pic_list_reordering_flag_l0, 0);
  EXPECT_EQ(header.ref_pic_list_reordering_flag_l1, 0);

  // pred weight table
  EXPECT_EQ(header.luma_log2_weight_denom, 0);
  EXPECT_EQ(header.chroma_log2_weight_denom, 0);


  EXPECT_EQ(header.cabac_init_idc, 0);
  EXPECT_EQ(header.slice_qp_delta, -2);
  EXPECT_EQ(header.sp_for_switch_flag, false);
  EXPECT_EQ(header.slice_qs_delta, 0);
  EXPECT_EQ(header.disable_deblocking_filter_idc, 0);
  EXPECT_EQ(header.slice_alpha_c0_offset_div2, 0);
  EXPECT_EQ(header.slice_beta_offset_div2, 0);
  EXPECT_EQ(header.slice_group_change_cycle, 0);

  EXPECT_EQ(header.no_output_of_prior_pics_flag, false);
  EXPECT_EQ(header.long_term_reference_flag, false);
  EXPECT_EQ(header.adaptive_ref_pic_marking_mode_flag, false);
}


