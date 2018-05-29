//
// Created by aLIEzTed on 5/14/18.
//

#ifndef SIMPLEH264_SPS_PARSER_H
#define SIMPLEH264_SPS_PARSER_H
#include "parser.h"
#include "util/bitutil.h"
#include <vector>
#ifdef _TEST
#include <gtest/gtest_prod.h>
#endif
class Sps_Parser : public Parser<SPS> {
 private:
  const int id = 7;


#ifdef _TEST
  FRIEND_TEST(SpsScalingListTest, SPS_Scaling_list);
#endif

  void scaling_list(unsigned char *data,
                   unsigned long& offset,
                   int& scalingList,
                   int sizeOfScalingList,
                   bool &useDefaultScalingMatrixFlag);
 public:
  Sps_Parser() : Parser() {}
  SPS parse(unsigned char *data, unsigned long len, unsigned long& offset) override;


  int getType() override;

  ~Sps_Parser() override;
};
#endif //SIMPLEH264_SPS_PARSER_H
