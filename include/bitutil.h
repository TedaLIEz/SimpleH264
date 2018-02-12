//
// Created by aLIEzTed on 2/11/18.
//

#ifndef SIMPLEH264_BITUTIL_H
#define SIMPLEH264_BITUTIL_H
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
inline long next_bit(unsigned char* bits, int len, int bit_len, int offset) {
  if (bit_len > len * 8 - offset) {
    std::cerr << "Out of boundary " << std::endl;
    return -1;
  }
  int start_index = offset / 8;
  int start_offset = offset % 8;
  auto start_bits = bits + start_index;
  long rst = 0;
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
      int end_len = bit_len / 8;
      int end_offset = bit_len % 8;
      for (int i = 0; i < end_len; i++) {
        rst += (*start_bits) << (bit_len - 8 * (i + 1));
        start_bits++;
      }
      int offset_value = (*start_bits) >> (8 - end_offset);
      rst += offset_value;
    }

  } else {
    int end_len = bit_len / 8;
    int end_offset = bit_len % 8;
    for (int i = 0; i < end_len; i++) {
      rst += ((int)(*start_bits)) << (bit_len - 8 * (i + 1));
      start_bits++;
    }
    int offset_value = (*start_bits) >> (8 - end_offset);
    rst += offset_value;
  }
  return rst;

}
}
#endif //SIMPLEH264_BITUTIL_H
