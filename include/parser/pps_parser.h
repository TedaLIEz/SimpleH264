//
// Created by aLIEzTed on 5/21/18.
//

#ifndef SIMPLEH264_PPS_PARSER_H
#define SIMPLEH264_PPS_PARSER_H
#include <h264/pps.h>
#include "parser.h"
class Pps_Parser : public Parser<PPS> {
 private:
  const int id = 8;
 public:
  Pps_Parser() : Parser() {}
  PPS parse(unsigned char *data, unsigned long len, unsigned long& offset) override;

  int getType() override;

  ~Sps_Parser() override;
};

#endif //SIMPLEH264_PPS_PARSER_H
