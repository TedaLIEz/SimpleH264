//
// Created by aLIEzTed on 5/21/18.
//

#ifndef SIMPLEH264_BITSTREAM_H
#define SIMPLEH264_BITSTREAM_H
#include <string>
#include "parser/nalu_parser.h"
class BitStream {
 private:
  NAL_Parser parser;
  unsigned long offset;
  int read_one_nalu(std::ifstream &file, unsigned long &offset, unsigned char *&nalu, unsigned long &nalu_size);
 public:
  void start_parse(const std::string &str);
  explicit BitStream() : parser{}, offset(0) {}
#ifndef DEBUG
#else
  unsigned long getOffset() const {
    return offset;
  }
#endif
};
#endif //SIMPLEH264_BITSTREAM_H
