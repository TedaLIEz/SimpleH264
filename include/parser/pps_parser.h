//
// Created by aLIEzTed on 5/21/18.
//

#ifndef SIMPLEH264_PPS_PARSER_H
#define SIMPLEH264_PPS_PARSER_H
#include <h264/pps.h>
#include "parser.h"
#ifdef _TEST
#include <gtest/gtest_prod.h>
#endif
class Pps_Parser : public Parser<PPS> {
 private:
  const int id = 8;
#ifdef _TEST
  FRIEND_TEST(PpsMoreRbspDataTest, More_RBSP_Data_Test);
#endif


 public:
  Pps_Parser() : Parser() {}
  PPS parse(unsigned char *data, unsigned long len, unsigned long &offset) override;

  int getType() override;

  ~Pps_Parser() override;
};

#endif //SIMPLEH264_PPS_PARSER_H
