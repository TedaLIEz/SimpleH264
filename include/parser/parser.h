//
// Created by aLIEzTed on 5/14/18.
//

#ifndef SIMPLEH264_PARSER_H
#define SIMPLEH264_PARSER_H
#include <sstream>
#include "h264/sps.h"
#include "util/golomb.h"
template <class T>
class Parser {
 public:
  Parser() = default;
  virtual T parse(unsigned char* data, unsigned long len, unsigned long& offset) = 0;
  virtual int getType() = 0;
  virtual ~Parser() = default;
  int uev_decode(unsigned char* data, unsigned long& offset, const std::string& name) {
    long len = -1;
    auto rst = golomb::get_uev_decode(data, offset, len);
    std::ostringstream oss;
    oss << "Error decoding " << name << " at offset " << offset;
    ASSERT(len > 0, oss.str());
    offset += len;
    return rst;
  }

  int sev_decode(unsigned char* data, unsigned long& offset, const std::string& name) {
    long len = -1;
    auto rst = golomb::get_sev_decode(data, offset, len);
    std::ostringstream oss;
    oss << "Error decoding " << name << " at offset " << offset;
    ASSERT(len > 0, oss.str());
    offset += len;
    return rst;
  }
};


#endif //SIMPLEH264_PARSER_H
