//
// Created by aLIEzTed on 5/19/18.
//

#ifndef SIMPLEH264_VUI_PARSER_H
#define SIMPLEH264_VUI_PARSER_H
#include "parser.h"
#include "h264/vui.h"
#define Extended_SAR 255
class VUI_Parser : public Parser<VUI> {
 public:
  VUI parse(unsigned char* data, unsigned long len, unsigned long& offset) override ;
  int getType() override ;
};
#endif //SIMPLEH264_VUI_PARSER_H
