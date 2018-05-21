//
// Created by aLIEzTed on 5/15/18.
//

#include <nalu_parser.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstring>


TEST(NAL_Parse_Header_Test, ParseHeader_Test) {
  unsigned char mock_data[] = {0x67};
  unsigned long offset = 0;
  auto parser = new NAL_Parser();
  auto header = parser->parse_header(mock_data, offset);
  EXPECT_EQ(header.nal_unit_type, 7);
  EXPECT_EQ(header.nal_ref_idc, 3);
  EXPECT_EQ(header.forbidden_zero_bit, 0);
}

TEST(NAL, SODB_Single_Test) {
  unsigned char mock_data[] = {0x00, 0x00, 0x03, 0x01};
  unsigned long size = 4;
  read_one_sodb(mock_data, size);
  EXPECT_EQ(size, 3);
  unsigned char rst[3];
  memcpy(rst, mock_data, size);
  EXPECT_THAT(rst, ::testing::ElementsAre(0x00, 0x00, 0x01));
}

TEST(NAL, SODB_Multiple_Test) {
  unsigned char mock_data[] = {0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 0x03, 0x00};
  unsigned long size = 8;
  read_one_sodb(mock_data, size);
  EXPECT_EQ(size, 6);
  unsigned char rst[6];
  memcpy(rst, mock_data, size);
  EXPECT_THAT(rst, ::testing::ElementsAre(0x00, 0x00, 0x01, 0x00, 0x00, 0x00));
}