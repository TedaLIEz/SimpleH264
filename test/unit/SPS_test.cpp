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

TEST_F(SpsTest, SPS_Parse_Profile_Test) {
  EXPECT_EQ(spsParser->getProfile_idc(), 0x64);
}

TEST_F(SpsTest, SPS_Parse_Level_Test) {
  EXPECT_EQ(spsParser->getLevel_idc(), 0x29);
}

TEST_F(SpsTest, SPS_Parse_Seq_param_Test) {
  EXPECT_EQ(spsParser->getSeq_param_set_id(), 0);
}

