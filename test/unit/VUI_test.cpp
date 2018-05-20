//
// Created by aLIEzTed on 5/19/18.
//
#include "vui.h"
#include "vui_parser.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class VuiTestCase : public ::testing::Test {
 protected:
  static VUI vui;

  static void SetUpTestCase() {
    auto vuiParser = new VUI_Parser();
    // vui starts at 704 / 8 = 88
    // 1 + 8 + 1 + 1 + 3 + 1 + 1 + 1 =
    unsigned char mock_data[28] =
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
         0x8c,   // 0b10001100
         0xb0};  // 0b10110000
    unsigned long offset = 83;
    vui = vuiParser->parse(mock_data, 28, offset);
    delete vuiParser;
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

VUI VuiTestCase::vui;

TEST_F(VuiTestCase, VUI_aspect_ratio_info_present_flag) {
  EXPECT_EQ(vui.aspect_ratio_info_present_flag, 1);
}

TEST_F(VuiTestCase, VUI_aspect_ratio_idc) {
  EXPECT_EQ(vui.aspect_ratio_idc, 1);
}

TEST_F(VuiTestCase, VUI_overscan_info_present_flag) {
  EXPECT_EQ(vui.overscan_info_present_flag, 0);
}

TEST_F(VuiTestCase, VUI_video_signal_type_present_flag) {
  EXPECT_EQ(vui.video_signal_type_present_flag, 1);
}

TEST_F(VuiTestCase, VUI_video_format) {
  EXPECT_EQ(vui.video_format, 5);
}

TEST_F(VuiTestCase, VUI_video_full_range_flag) {
  EXPECT_EQ(vui.video_full_range_flag, 0);
}

TEST_F(VuiTestCase, VUI_colour_description_present_flag) {
  EXPECT_EQ(vui.colour_description_present_flag, 1);
}

TEST_F(VuiTestCase, VUI_colour_primaries) {
  EXPECT_EQ(vui.colour_primaries, 1);
}

TEST_F(VuiTestCase, VUI_transfer_characteristics) {
  EXPECT_EQ(vui.transfer_characteristics, 1);
}

TEST_F(VuiTestCase, VUI_matrix_coefficients) {
  EXPECT_EQ(vui.matrix_coefficients, 1);
}

TEST_F(VuiTestCase, VUI_chroma_loc_info_present_flag) {
  EXPECT_EQ(vui.chroma_loc_info_present_flag, 0);
}

TEST_F(VuiTestCase, VUI_timing_info_present_flag) {
  EXPECT_EQ(vui.timing_info_present_flag, 1);
}

TEST_F(VuiTestCase, VUI_num_units_in_tick) {
  EXPECT_EQ(vui.num_units_in_tick, 1001);
}

TEST_F(VuiTestCase, VUI_time_scale) {
  EXPECT_EQ(vui.time_scale, 48000);
}

TEST_F(VuiTestCase, VUI_fixed_frame_rate_flag) {
  EXPECT_EQ(vui.fixed_frame_rate_flag, 1);
}

TEST_F(VuiTestCase, VUI_nal_hrd_parameters_present_flag) {
  EXPECT_EQ(vui.nal_hrd_parameters_present_flag, 0);
}


TEST_F(VuiTestCase, VUI_vcl_hrd_parameters_present_flag) {
  EXPECT_EQ(vui.vcl_hrd_parameters_present_flag, false);
}

TEST_F(VuiTestCase, VUI_bitstream_restriction_flag) {
  EXPECT_EQ(vui.bitstream_restriction_flag, true);
}

TEST_F(VuiTestCase, VUI_motion_vectors_over_pic_boundaries_flag) {
  EXPECT_EQ(vui.motion_vectors_over_pic_boundaries_flag, true);
}

TEST_F(VuiTestCase, VUI_max_bytes_per_pic_denom) {
  EXPECT_EQ(vui.max_bytes_per_pic_denom, 0);
}

TEST_F(VuiTestCase, VUI_max_bits_per_mb_denom) {
  EXPECT_EQ(vui.max_bits_per_mb_denom, 0);
}

TEST_F(VuiTestCase, VUI_log2_max_mv_length_horizontal) {
  EXPECT_EQ(vui.log2_max_mv_length_horizontal, 11);
}

TEST_F(VuiTestCase, VUI_log2_max_mv_length_vertical) {
  EXPECT_EQ(vui.log2_max_mv_length_vertical, 11);
}

TEST_F(VuiTestCase, VUI_num_reorder_frames) {
  EXPECT_EQ(vui.max_num_reorder_frames, 2);
}

TEST_F(VuiTestCase, VUI_max_dec_frame_buffering) {
  EXPECT_EQ(vui.max_dec_frame_buffering, 4);
}

