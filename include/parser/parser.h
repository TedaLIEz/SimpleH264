//
// Created by aLIEzTed on 5/14/18.
//

#ifndef SIMPLEH264_PARSER_H
#define SIMPLEH264_PARSER_H
#include <sstream>
#include <context.h>
#include "h264/sps.h"
#include "util/golomb.h"
template<class T>
class Parser {
 public:
  Parser() = default;
  virtual T parse(unsigned char *data, unsigned long len, unsigned long &offset) = 0;
  virtual int getType() = 0;
  virtual ~Parser() = default;
  int uev_decode(unsigned char *data, unsigned long &offset, const std::string &name) {
    long len = -1;
    auto rst = golomb::get_uev_decode(data, offset, len);
    std::ostringstream oss;
    oss << "Error decoding " << name << " at offset " << offset;
    ASSERT(len > 0, oss.str());
    offset += len;
    return rst;
  }

  int sev_decode(unsigned char *data, unsigned long &offset, const std::string &name) {
    long len = -1;
    auto rst = golomb::get_sev_decode(data, offset, len);
    std::ostringstream oss;
    oss << "Error decoding " << name << " at offset " << offset;
    ASSERT(len > 0, oss.str());
    offset += len;
    return rst;
  }

  bool get_bool(unsigned char *data, unsigned long &offset) {
    return static_cast<bool>(bit::get_bit(data, offset++));
  }

  unsigned long read_bit(unsigned char *data, unsigned long bit_len, unsigned long &offset) {
    auto rst = bit::next_bit(data, bit_len, offset);
    offset += bit_len;
    return rst;
  }

  /**
 * return 0 if we have more rbsp data, else return the length of trailing zeros
 *
 * @param data the encoded data store in unsigned char array
 * @param len  length of data in bytes
 * @param offset offset by bits
 * @return 0 if we have more rbsp data, else return the length of trailing zeros
 */
  int more_rbsp_data(unsigned char *data, unsigned long len, unsigned long offset) {
    auto trailing_len = len * 8 - offset;
    auto trailing_rst = bit::next_bit(data, trailing_len, offset);
    auto mask = 1 << (trailing_len - 1);
    if (mask != trailing_rst) return 0;
    else return trailing_len;
  }
};

#endif //SIMPLEH264_PARSER_H
