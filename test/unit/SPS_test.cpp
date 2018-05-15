//
// Created by aLIEzTed on 5/15/18.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "sps.h"
TEST(SPS, SPS_Parse_Test) {
  unsigned char mock_data[] = {0x64,
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
  auto spsParser = new SpsParser();
  spsParser->parse(mock_data, 28);
  EXPECT_EQ(spsParser->getProfile_idc(), 0x64);
  EXPECT_EQ(spsParser->getLevel_idc(), 0x29);
}

