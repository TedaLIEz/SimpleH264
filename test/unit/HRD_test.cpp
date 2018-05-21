//
// Created by aLIEzTed on 5/19/18.
//
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "h264/hrd.h"
#include "parser/hrd_parser.h"
class HrdTestCase : public ::testing::Test {
 protected:
  static HRD hrd;

  static void SetUpTestCase() {
    auto hrdParser = new HRD_Parser();
    // vui starts at 704 / 8 = 88
    unsigned char mock_data[23] =
        {0x42,
         0xe0,
         0x15,
         0xa9,
         0x18,
         0x3c,
         0x11,
         0xfd,
         0x60,
         0x2d,
         0x41,
         0x80,
         0x41,
         0xad,
         0xb7,
         0xa0,
         0x0f,
         0x48,
         0x0f,
         0x55,
         0xef,
         0x7c,
         0x04};    // 0b 10100000
    unsigned long offset = 115;
    hrd = hrdParser->parse(mock_data, 23, offset);
    delete hrdParser;
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

HRD HrdTestCase::hrd;

TEST_F(HrdTestCase, HRD_cpb_cnt_minus1) {
  EXPECT_EQ(hrd.cpb_cnt_minus1, 0);
}

TEST_F(HrdTestCase, HRD_bit_rate_scale) {
  EXPECT_EQ(hrd.bit_rate_scale, 7);
}

TEST_F(HrdTestCase, HRD_cpb_size_scale) {
  EXPECT_EQ(hrd.cpb_size_scale, 10);
}

TEST_F(HrdTestCase, HRD_bit_rate_value_minus1) {
  EXPECT_EQ(hrd.bit_rate_value_minus1.size(), 1);
  EXPECT_THAT(hrd.bit_rate_value_minus1, ::testing::ElementsAre(488));
}

TEST_F(HrdTestCase, HRD_cpb_size_value_minus1) {
  EXPECT_EQ(hrd.cpb_size_value_minus1.size(), 1);
  EXPECT_THAT(hrd.cpb_size_value_minus1, ::testing::ElementsAre(244));
}

TEST_F(HrdTestCase, HRD_cbr_flag) {
  EXPECT_EQ(hrd.cbr_flag.size(), 1);
  EXPECT_THAT(hrd.cbr_flag, ::testing::ElementsAre(false));
}

TEST_F(HrdTestCase, HRD_initial_cpb_removal_delay_length_minus1) {
  EXPECT_EQ(hrd.initial_cpb_removal_delay_length_minus1, 23);
}

TEST_F(HrdTestCase, HRD_cpb_removal_delay_length_minus1) {
  EXPECT_EQ(hrd.cpb_removal_delay_length_minus1, 23);
}

TEST_F(HrdTestCase, HRD_dpb_output_delay_length_minus1) {
  EXPECT_EQ(hrd.dpb_output_delay_length_minus1, 23);
}

TEST_F(HrdTestCase, HRD_time_offset_length) {
  EXPECT_EQ(hrd.time_offset_length, 24);
}

