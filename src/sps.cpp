//
// Created by aLIEzTed on 5/14/18.
//
#include "sps.h"
void SpsParser::parse(unsigned char *data, unsigned long len) {
  int offset = 0;
  profile_idc = bit::read_bytes(data, len, offset);
  offset += 8;
  auto flags = static_cast<uint8_t>(bit::next_bit(data, len, 8, offset));
  offset += 8;
  auto cons_set0_flag = bit::get_bit(flags, 0);
  auto cons_set1_flag = bit::get_bit(flags, 1);
  auto cons_set2_flag = bit::get_bit(flags, 2);
  auto cons_set3_flag = bit::get_bit(flags, 3);
  auto cons_set4_flag = bit::get_bit(flags, 4);
  auto cons_set5_flag = bit::get_bit(flags, 5);
  auto zero_bits_mask = (1 << 2) - 1;
  auto reserved_zero_bits = flags & zero_bits_mask;
  if (reserved_zero_bits != 0) {
    std::cerr << "Assert failed " << std::endl;
    return;
  }
#ifdef DEBUG
  std::cout << "set0_flag " << cons_set0_flag << std::endl;
  std::cout << "set1_flag " << cons_set1_flag << std::endl;
  std::cout << "set2_flag " << cons_set2_flag << std::endl;
  std::cout << "set3_flag " << cons_set3_flag << std::endl;
  std::cout << "set4_flag " << cons_set4_flag << std::endl;
  std::cout << "set5_flag " << cons_set5_flag << std::endl;
#endif
  level_idc = bit::read_bytes(data, len, offset);
  offset += 8;
}

int SpsParser::getProfile_idc() const {
  return profile_idc;
}

int SpsParser::getLevel_idc() const {
  return level_idc;
}

int SpsParser::getType() {
  return id;
}