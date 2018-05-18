//
// Created by aLIEzTed on 5/15/18.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "sps.h"

class SpsTest : public ::testing::Test {
 protected:
  // Some expensive resource shared by all tests.
  static SpsParser *spsParser;
  unsigned char mock_data[28] = {0x64,
                                     0x00,
                                     0x29,
                                     0xac,   // 0b10101100
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
                                     0xb0,};
  // Per-test-case set-up.
  // Called before the first test in this test case.
  // Can be omitted if not needed.
  static void SetUpTestCase() {
    spsParser = new SpsParser();
  }

  // Per-test-case tear-down.
  // Called after the last test in this test case.
  // Can be omitted if not needed.
  static void TearDownTestCase() {
    delete spsParser;
    spsParser = NULL;
  }



  // You can define per-test set-up and tear-down logic as usual.
  virtual void SetUp() {
    spsParser->parse(mock_data, 28);
  }
  virtual void TearDown() {

  }


};

SpsParser* SpsTest::spsParser = NULL;

TEST_F(SpsTest, SPS_Profile_idc) {
  EXPECT_EQ(spsParser->getProfile_idc(), 0x64);
}

TEST_F(SpsTest, SPS_Level_idc) {
  EXPECT_EQ(spsParser->getLevel_idc(), 0x29);
}

TEST_F(SpsTest, SPS_Seq_param_set_id) {
  EXPECT_EQ(spsParser->getSeq_param_set_id(), 0);
}

TEST_F(SpsTest, SPS_Chroma_format_idc) {
  EXPECT_EQ(spsParser->getChroma_format_idc(), 1);
}


// private test starts
TEST_F(SpsTest, SPS_Separate_colour_plane_flag) {
  EXPECT_EQ(spsParser->separate_colour_plane_flag, -1);
}

TEST_F(SpsTest, SPS_Depth_luma) {
  EXPECT_EQ(spsParser->bit_depth_luma_minus8, 0);
}

TEST_F(SpsTest, SPS_Depth_chroma) {
  EXPECT_EQ(spsParser->bit_depth_chroma_minus8, 0);
}

TEST_F(SpsTest, SPS_Qpprime_y_zero_transform_bypass_flag) {
  EXPECT_EQ(spsParser->qpprime_y_zero_transform_bypass_flag, false);
}

TEST_F(SpsTest, SPS_Seq_scaling_matrix_present_flag) {
  EXPECT_EQ(spsParser->seq_scaling_matrix_present_flag, false);
}

// private test ends


