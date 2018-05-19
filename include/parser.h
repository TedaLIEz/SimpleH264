//
// Created by aLIEzTed on 5/14/18.
//

#ifndef SIMPLEH264_PARSER_H
#define SIMPLEH264_PARSER_H
#include "sps.h"
template <class T>
class Parser {
 public:
  Parser() = default;
  virtual T parse(unsigned char* data, unsigned long len) = 0;
  virtual int getType() = 0;
  virtual ~Parser() = default;;
};


#endif //SIMPLEH264_PARSER_H
