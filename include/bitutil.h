//
// Created by aLIEzTed on 2/11/18.
//

#ifndef SIMPLEH264_BITUTIL_H
#define SIMPLEH264_BITUTIL_H
#include <iostream>
namespace bit {
/**
 * find bits of bit_len length at offset in the bits array
 *
 * @param bits bits stored as unsigned char array
 * @param len length of bits
 * @param bit_len the length of result
 * @param offset start offset of bits
 * @return bits represented as long
 */

inline unsigned long next_bit(char* bits, unsigned long len, long bit_len, int offset) {
  if (bit_len > len * 8 - offset) {
    std::cerr << "Out of boundary " << std::endl;
    return 0;
  }
  int start_index = offset / 8;
  int start_offset = offset % 8;
  auto start_bits = bits + start_index;
  unsigned rst = 0;
  if (start_offset != 0) {
    if (bit_len < 8 - start_offset) {
      auto bit_mask = static_cast<unsigned char>((1 << bit_len) - 1);
      long tmp = *start_bits >> (8 - start_offset - bit_len);
      rst += (tmp & bit_mask);
    } else {
      auto bit_mask = static_cast<unsigned char>((1 << (8 - start_offset)) - 1);
      rst += (*start_bits & bit_mask) << (bit_len - 8 + start_offset);
      start_bits++;
      bit_len = bit_len - 8 + start_offset;
      long end_len = bit_len / 8;
      long end_offset = bit_len % 8;
      for (int i = 0; i < end_len; i++) {
        rst += (*start_bits) << (bit_len - 8 * (i + 1));
        start_bits++;
      }
      int offset_value = (*start_bits) >> (8 - end_offset);
      rst += offset_value;
    }

  } else {
    long end_len = bit_len / 8;
    long end_offset = bit_len % 8;
    for (int i = 0; i < end_len; i++) {
      rst += ((unsigned long)(*start_bits)) << (bit_len - 8 * (i + 1));
      start_bits++;
    }
    int offset_value = (*start_bits) >> (8 - end_offset);
    rst += offset_value;
  }
  return rst;

}

/**
 * return one byte of bits from bits array at offset
 * @param bits bits array
 * @param len total length of bits array
 * @param offset offset of the result
 * @return one byte of bits from bits array at offset
 */
inline uint8_t read_bytes(char* bits, unsigned long len, int offset) {
  return static_cast<uint8_t>(next_bit(bits, len, 8, offset));
}

inline uint8_t get_bit(unsigned char* bits, uint8_t bytePos, uint8_t bitPos) {
  uint8_t mask = 0, val = 0;

  mask = 1 << (7 - bitPos);
  val = ((bits[bytePos] & mask) != 0);
  return val;
}

inline uint8_t write_bit(unsigned char* bits, long pos, bool bit) {
  auto byte_offset = pos / 8;
  auto offset = pos % 8;
  bits[byte_offset] += static_cast<unsigned char>(bit << (8 - offset - 1));
}
}
#endif //SIMPLEH264_BITUTIL_H
