//
// Created by aLIEzTed on 2/24/18.
//

#ifndef SIMPLEH264_NALU_PARSER_H
#define SIMPLEH264_NALU_PARSER_H
#include <nalu.h>
#include <fstream>
#include "parser.h"
#ifdef _TEST
#include <gtest/gtest_prod.h>
#endif
class NAL_Parser : public Parser<NALU> {
 public:
  NALU parse(unsigned char* data, unsigned long len, unsigned long& offset) override;
  int getType() override;
 private:
#ifdef _TEST
  FRIEND_TEST(NAL_Parse_Header_Test, ParseHeader_Test);
#endif

  NALU_header parse_header(unsigned char* data, unsigned long& offset);
};

/**
 * parse the nalu to sodb, where 0x000000, 0x000001, 0x000002 and 0x000003 turns to be 0x00000300,
 * 0x00000301, 0x00000302 and 0x00000303 respectively.
 * @param nalu the original nalu, change to sodb after parsing
 * @param nalu_size the length of nalu in bytes, change to length of sodb after parsing
 * @return 0 parsing successfully
 */
int read_one_sodb(unsigned char* nalu, unsigned long& nalu_size);
int read_one_nalu(std::ifstream& file, unsigned long start, unsigned char*& nalu, unsigned long& nalu_size);


#endif //SIMPLEH264_NALU_PARSER_H
