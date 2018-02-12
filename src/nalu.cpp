//
// Created by aLIEzTed on 2/11/18.
//

#include "nalu.h"
#include "bitutil.h"
int find_nal_prefix(unsigned char* bits, unsigned long bit_len, std::vector<uint8_t> &nalBytes) {
  int offset = 0;
  int count = 0;
  while (offset < bit_len) {
    while (bit::next_bit(bits, bit_len, 24, offset) != NAL_HEADER_SHORT
        && bit::next_bit(bits, bit_len, 32, offset) != NAL_HEADER_LONG) {
      offset += 8;
      // find leading_zero_8bits
    }
    if (bit::next_bit(bits, bit_len, 24, offset) != NAL_HEADER_SHORT) {
      offset += 8;
      // find zero byte
    }
    auto header = bit::next_bit(bits, bit_len, 24, offset);
    if (header != NAL_HEADER_SHORT) {
      std::cerr << "Invalid nalu " << std::endl;
      return -1;
    }
    // find start_code_prefix_one_3bytes
    // find nalu header
    offset += 24;
    while (offset < bit_len && bit::next_bit(bits, bit_len, 24, offset) != NAL_HEADER_SHORT
        && bit::next_bit(bits, bit_len, 32, offset) != NAL_HEADER_LONG) {
      auto tmp = bit::read_bytes(bits, bit_len, offset);
      nalBytes.push_back(tmp);
      offset += 8;
      count++;
    }
  }
  return count;
}