//
// Created by aLIEzTed on 5/15/18.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "sps_parser.h"
#include "sps.h"

class SpsTestCase : public ::testing::Test {
 protected:
  // Some expensive resource shared by all tests.
  static Sps sps, spsTest2;
  // Per-test-case set-up.
  // Called before the first test in this test case.
  // Can be omitted if not needed.
  static void SetUpTestCase() {
    auto spsParser1 = new Sps_Parser();
    unsigned char mock_data1[28] =
        {0x64,
         0x00,
         0x29,
         0xac,
         0xc8,
         0x50,
         0x1e,
         0x00,
         0x89,
         0xf9,
         0x70,
         0x16,
         0xa0,
         0x20,
         0x20,
         0x28,
         0x00,
         0x00,
         0x1f,
         0x48,
         0x00,
         0x05,
         0xdc,
         0x04,
         0x78,
         0xc1,
         0x8c,
         0xb0};    // 0b 10100000
    unsigned long offset = 0;
    sps = spsParser1->parse(mock_data1, 28, offset);
    ASSERT_EQ(offset, 28 * 8);
    delete spsParser1;
  }

  // Per-test-case tear-down.
  // Called after the last test in this test case.
  // Can be omitted if not needed.
  static void TearDownTestCase() {
  }

  // You can define per-test set-up and tear-down logic as usual.
  virtual void SetUp() {

  }
  virtual void TearDown() {

  }

};

Sps SpsTestCase::sps;

TEST_F(SpsTestCase, SPS1_Profile_idc) {
  EXPECT_EQ(sps.profile_idc, 0x64);
}

TEST_F(SpsTestCase, SPS1_Level_idc) {
  EXPECT_EQ(sps.level_idc, 0x29);
}

TEST_F(SpsTestCase, SPS1_Seq_param_set_id) {
  EXPECT_EQ(sps.seq_parameter_set_id, 0);
}

TEST_F(SpsTestCase, SPS1_Chroma_format_idc) {
  EXPECT_EQ(sps.chroma_format_idc, 1);
}

TEST_F(SpsTestCase, SPS1_Separate_colour_plane_flag) {
  EXPECT_EQ(sps.separate_colour_plane_flag, -1);
}

TEST_F(SpsTestCase, SPS1_Depth_luma) {
  EXPECT_EQ(sps.bit_depth_luma_minus8, 0);
}

TEST_F(SpsTestCase, SPS1_Depth_chroma) {
  EXPECT_EQ(sps.bit_depth_chroma_minus8, 0);
}

TEST_F(SpsTestCase, SPS1_Qpprime_y_zero_transform_bypass_flag) {
  EXPECT_EQ(sps.qpprime_y_zero_transform_bypass_flag, false);
}

TEST_F(SpsTestCase, SPS1_Seq_scaling_matrix_present_flag) {
  EXPECT_EQ(sps.seq_scaling_matrix_present_flag, 0);
}

TEST_F(SpsTestCase, SPS1_Scaling_list_size_Test) {
  EXPECT_EQ(sps.seq_scaling_list_present_flag.size(), 0);
}


TEST_F(SpsTestCase, SPS1_Log2_max_frame_num_minus4) {
  EXPECT_EQ(sps.log2_max_frame_num_minus4, 0);
}

TEST_F(SpsTestCase, SPS1_Pic_order_cnt_type) {
  EXPECT_EQ(sps.pic_order_cnt_type, 0);
}

TEST_F(SpsTestCase, SPS1_log2_max_pic_order_cnt_lsb_minus4) {
  EXPECT_EQ(sps.log2_max_pic_order_cnt_lsb_minus4, 3);
}

TEST_F(SpsTestCase, SPS1_delta_pic_order_always_zero_flag) {
  EXPECT_EQ(sps.delta_pic_order_always_zero_flag, false);
}

TEST_F(SpsTestCase, SPS1_offset_for_non_ref_pic) {
  EXPECT_EQ(sps.offset_for_non_ref_pic, -1);
}

TEST_F(SpsTestCase, SPS1_offset_for_top_to_bottom_field) {
  EXPECT_EQ(sps.offset_for_top_to_bottom_field, -1);
}

TEST_F(SpsTestCase, SPS1_num_ref_frames_in_pic_order_cnt_cycle) {
  EXPECT_EQ(sps.num_ref_frames_in_pic_order_cnt_cycle, -1);
}


TEST_F(SpsTestCase, SPS1_max_num_ref_frames) {
  EXPECT_EQ(sps.max_num_ref_frames, 4);
}



TEST_F(SpsTestCase, SPS1_gaps_in_frame_num_value_allowed_flag) {
  EXPECT_EQ(sps.gaps_in_frame_num_value_allowed_flag, 0);
}

TEST_F(SpsTestCase, SPS1_pic_width_in_mbs_minus1) {
  EXPECT_EQ(sps.pic_width_in_mbs_minus1, 119);
}

TEST_F(SpsTestCase, SPS1_pic_height_in_map_units_minus1) {
  EXPECT_EQ(sps.pic_height_in_map_units_minus1, 67);
}

TEST_F(SpsTestCase, SPS1_frame_mbs_only_flag) {
  EXPECT_EQ(sps.frame_mbs_only_flag, 1);
}

TEST_F(SpsTestCase, SPS1_direct_8x8_inference_flag) {
  EXPECT_EQ(sps.direct_8x8_inference_flag, 1);
}

TEST_F(SpsTestCase, SPS1_frame_cropping_flag) {
  EXPECT_EQ(sps.frame_cropping_flag, 1);
}

TEST_F(SpsTestCase, SPS1_frame_crop_left_offset) {
  EXPECT_EQ(sps.frame_crop_left_offset, 0);
}


TEST_F(SpsTestCase, SPS1_frame_crop_right_offset) {
  EXPECT_EQ(sps.frame_crop_right_offset, 0);
}

TEST_F(SpsTestCase, SPS1_frame_crop_top_offset) {
  EXPECT_EQ(sps.frame_crop_top_offset, 0);
}

TEST_F(SpsTestCase, SPS1_frame_crop_bottom_offset) {
  EXPECT_EQ(sps.frame_crop_bottom_offset, 4);
}


TEST_F(SpsTestCase, SPS1_vui_parameters_present_flag) {
  EXPECT_EQ(sps.vui_parameters_present_flag, 1);
}

TEST(SpsScalingListTest, SPS_Scaling_list) {
  auto *spsParser = new Sps_Parser();
  auto *scaling_list = new int[16]();
  auto *use_default_scaling_matrix_flag = new bool[16]();
  unsigned char data[] = {
      0x64, 0x00, 0x28, 0xAD, /* 0b 1 010 1 1 0 1 */ 0x84  /* 0b 1000 0100 */, 0x05, /* 0b0000 0101*/0x45, 0x62,
      0xb8, /* 0b 1011 1000 */0xAC, 0x54, 0x74, 0x20, 0x2A,
      0x2B, 0x15, 0xC5, 0x62, 0xA3, 0xA1, 0x01, 0x51, 0x58, 0xAE, 0x2B, 0x15, 0x1D, 0x08,
      0x0A, 0x8A, 0xC5, 0x71, 0x58, 0xA8, 0xE8, 0x40, 0x54, 0x56, 0x2B, 0x8A, 0xC5, 0x47,
      0x42, 0x02, 0xA2, 0xB1, 0x5C, 0x56, 0x2A, 0x3A, 0x10, 0x24, 0x85, 0x21, 0x39, 0x3C,
      0x9F, 0x27, 0xE4, 0xFE, 0x4F, 0xC9, 0xF2, 0x79, 0xB9, 0xB3, 0x4D, 0x08, 0x12, 0x42,
      0x90, 0x9C, 0x9E, 0x4F, 0x93, 0xF2, 0x7F, 0x27, 0xE4, 0xF9, 0x3C, 0xDC, 0xD9, 0xA6,
      0xB4, 0x05, 0x01, 0xED, 0x80, 0xAA, 0x40, 0x00, 0x00, 0x03, 0x00, 0x40, 0x00, 0x00,
      0x25, 0x98, 0x10, 0x00, 0xFA, 0x00, 0x00, 0x46, 0x51, 0xBD, 0xEF, 0x85, 0xE1, 0x10,
      0x8D, 0x40
  };
  unsigned long offset = 33;
  spsParser->scaling_list(data, offset, scaling_list[0], 16, use_default_scaling_matrix_flag[0]);
}






