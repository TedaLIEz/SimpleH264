//
// Created by aLIEzTed on 5/14/18.
//

#ifndef SIMPLEH264_SPS_H
#define SIMPLEH264_SPS_H
#include "parser.h"
#include "bitutil.h"
class SpsParser : public Parser {
 private:
  const int id = 7;
  int profile_idc = -1;
  int level_idc = -1;
  int seq_parameter_set_id = -1;
 public:
  SpsParser() : Parser() {}
  void parse(unsigned char* data, unsigned long len) override;

  int getProfile_idc() const;

  int getType() override;

};
#endif //SIMPLEH264_SPS_H
