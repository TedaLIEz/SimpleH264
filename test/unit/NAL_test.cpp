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


TEST(NAL_SODB_Test, SODB_Single_Test) {
  unsigned char mock_data[] = {0x00, 0x00, 0x03, 0x01};
  unsigned long size = 4;
  auto parser = new NAL_Parser();
  auto rst = parser->sodb(mock_data, size);
  ASSERT_EQ(rst, 0);
  EXPECT_EQ(size, 3);
  unsigned char tmp[3];
  memcpy(tmp, mock_data, size);
  EXPECT_THAT(tmp, ::testing::ElementsAre(0x00, 0x00, 0x01));
  delete parser;
}

TEST(NAL_SODB_Test, SODB_Multiple_Test) {
  unsigned char mock_data[] = {0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 0x03, 0x00};
  unsigned long size = 8;
  auto parser = new NAL_Parser();
  auto rst = parser->sodb(mock_data, size);
  ASSERT_EQ(rst, 0);
  EXPECT_EQ(size, 6);
  unsigned char tmp[6];
  memcpy(tmp, mock_data, size);
  EXPECT_THAT(tmp, ::testing::ElementsAre(0x00, 0x00, 0x01, 0x00, 0x00, 0x00));
}

TEST(NAL_Test, NAL_Parse_Test) {
  unsigned char mock_data[] = {0x67, 0x00, 0x00, 0x03, 0x01};
  unsigned long size = 5;
  auto parser = new NAL_Parser();
  unsigned long offset = 0;
  auto nal = parser->parse(mock_data, size, offset);
  EXPECT_EQ(offset, 40);
  EXPECT_EQ(nal.size, 4);
  EXPECT_EQ(nal.header.forbidden_zero_bit, 0);
  EXPECT_EQ(nal.header.nal_ref_idc, 3);
  EXPECT_EQ(nal.header.nal_unit_type, 7);
}