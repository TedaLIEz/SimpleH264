//
// Created by aLIEzTed on 5/19/18.
//

#ifndef SIMPLEH264_HRD_PARSER_H
#define SIMPLEH264_HRD_PARSER_H
#include "parser.h"
#include "hrd.h"
class HRD_Parser : public Parser<HRD> {
 public:
  HRD parse(unsigned char *data, unsigned long len, unsigned long& offset) override;
  int getType() override;
};
#endif //SIMPLEH264_HRD_PARSER_H
