//
// Created by aLIEzTed on 2/23/18.
//

#ifndef SIMPLEH264_GOLOMB_H
#define SIMPLEH264_GOLOMB_H

#include <cstdint>
#include "bitutil.h"
namespace golomb {
inline int get_uev_encode(unsigned int code_num, unsigned char* bits, long& len) {
  int tmp = code_num + 1;
  uint8_t bit_len = 0;
  while (tmp > 0) {
    tmp = (tmp >> 1);
    bit_len++;
  }
  len = bit_len * 2 - 1;
  for (int i = bit_len - 1; i < len; i++) {
    int mask = 1 << (len - i - 1);
    bool bit = ((code_num + 1) & mask) != 0;
    bit::write_bit(bits, i, bit);
  }
  return 0;
}

inline int get_uev_decode(unsigned char* buf, uint8_t bytePos, uint8_t bitPos) {
  if (bitPos > 8) {
    std::cerr << "bitPos out of boundary!!!" << std::endl;
    return -1;
  }
  uint8_t bit = 0, prefixZeroCnt = 0;
  int rst = 0;
  while (true) {
    bit = bit::get_bit(buf, bytePos, bitPos);
    if (bit == 0) {
      prefixZeroCnt++;
      bitPos++;
    } else {
      break;
    }
  }
#ifdef DEBUG
  std::cout << "method: get_uev_decode " << std::endl;
  std::cout << "prefixZeroCnt " << (int)prefixZeroCnt << std::endl;
#endif
  for (int i = 0; i < prefixZeroCnt + 1; i++) {
    bit = bit::get_bit(buf, bytePos, bitPos++);
    int tmp = bit << (prefixZeroCnt - i);
    rst += tmp;
  }
  return rst - 1;

}
}
#endif //SIMPLEH264_GOLOMB_H
