//
// Created by aLIEzTed on 5/22/18.
//

#include <h264/pps.h>
#include "gmock/gmock.h"
#include "parser/pps_parser.h"
class PpsTestCase : public ::testing::Test {
 protected:
  static PPS pps;

  static void SetUpTestCase() {
    auto ppsParser = new Pps_Parser();
    unsigned char mock_data[3] = {0xde, 0x09, 0x88};
    unsigned long offset = 0;
    pps = ppsParser->parse(mock_data, 3, offset);
    ASSERT_EQ(offset, 3 * 8);
    delete ppsParser;
  }

};

PPS PpsTestCase::pps;

TEST_F(PpsTestCase, PPS_pic_parameter_set_id) {
  EXPECT_EQ(pps.pic_parameter_set_id, 0);
}

TEST_F(PpsTestCase, PPS_seq_parameter_set_id) {
  EXPECT_EQ(pps.seq_parameter_set_id, 0);
}

TEST_F(PpsTestCase, PPS_entropy_coding_mode_flag) {
  EXPECT_EQ(pps.entropy_coding_mode_flag, false);
}

TEST_F(PpsTestCase, PPS_bottom_field_pic_order_in_frame_present_flag) {
  EXPECT_EQ(pps.bottom_field_pic_order_in_frame_present_flag, true);
}

TEST_F(PpsTestCase, PPS_num_slice_groups_minus1) {
  EXPECT_EQ(pps.num_slice_groups_minus1, 0);
}

TEST_F(PpsTestCase, PPS_slice_group_map_type) {
  EXPECT_EQ(pps.slice_group_map_type, 0);
}

TEST_F(PpsTestCase, PPS_num_ref_idx_l0_default_active_minus1) {
  EXPECT_EQ(pps.num_ref_idx_l0_default_active_minus1, 0);
}

TEST_F(PpsTestCase, PPS_num_ref_idx_l1_default_active_minus1) {
  EXPECT_EQ(pps.num_ref_idx_l1_default_active_minus1, 0);
}

TEST_F(PpsTestCase, PPS_weighted_pred_flag) {
  EXPECT_EQ(pps.weighted_pred_flag, false);
}

TEST_F(PpsTestCase, PPS_weighted_bipred_idc) {
  EXPECT_EQ(pps.weighted_bipred_idc, 0);
}

TEST_F(PpsTestCase, PPS_pic_init_qp_minus26) {
  EXPECT_EQ(pps.pic_init_qp_minus26, 2);
}

TEST_F(PpsTestCase, PPS_pic_init_qs_minus26) {
  EXPECT_EQ(pps.pic_init_qs_minus26, 0);
}

TEST_F(PpsTestCase, PPS_chroma_qp_index_offset) {
  EXPECT_EQ(pps.chroma_qp_index_offset, 0);
}

TEST_F(PpsTestCase, PPS_deblocking_filter_control_present_flag) {
  EXPECT_EQ(pps.deblocking_filter_control_present_flag, false);
}

TEST_F(PpsTestCase, PPS_constrained_intra_pred_flag) {
  EXPECT_EQ(pps.constrained_intra_pred_flag, false);
}

TEST_F(PpsTestCase, PPS_redundant_pic_cnt_present_flag) {
  EXPECT_EQ(pps.redundant_pic_cnt_present_flag, false);
}


TEST(PpsMoreRbspDataTest, More_RBSP_Data_Test) {
  auto parser = new Pps_Parser();
  unsigned char data[] = {0b10010000};
  EXPECT_EQ(parser->more_rbsp_data(data, 1, 3), 5);
  unsigned char data1[] = {0b10000000};
  EXPECT_EQ(parser->more_rbsp_data(data1, 1, 0), 8);
  unsigned char data2[] = {0b10100001};
  EXPECT_EQ(parser->more_rbsp_data(data2, 1, 2), 0);
  delete parser;
}

