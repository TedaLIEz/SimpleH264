//
// Created by aLIEzTed on 5/14/18.
//
#include <golomb.h>
#include "debug.h"
#include "sps.h"
void SpsParser::parse(unsigned char *data, unsigned long len) {
  unsigned long offset = 0;
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
  long seq_len = -1;
  seq_parameter_set_id = golomb::get_uev_decode(data, offset, seq_len);
  ASSERT(seq_len > 0, "Error getting length of decode of the seq_param_set_id");
  offset += seq_len;
  if (profile_idc == 100 || profile_idc == 110 || profile_idc == 122 || profile_idc == 244 || profile_idc == 44
      || profile_idc == 83 || profile_idc == 86 || profile_idc == 118 || profile_idc == 128 || profile_idc == 138
      || profile_idc == 139 || profile_idc == 134 || profile_idc == 135) {
#ifdef DEBUG
    std::cout << "profile_idc satisfied, find chroma_format_idc " << std::endl;
#endif
    long chroma_len = -1;
    chroma_format_idc = golomb::get_uev_decode(data, offset, chroma_len);
    ASSERT(chroma_len > 0, "Error getting length of decode of the chroma_format_idc");
    offset += chroma_len;
    if (chroma_format_idc == 3) {
      separate_colour_plane_flag = bit::get_bit(data, offset);
      ASSERT(separate_colour_plane_flag == 0 || separate_colour_plane_flag == 1, "Invalid separate_colour_plane_flag");
      offset += 1;
    }

  }
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

int SpsParser::getSeq_param_set_id() const {
  return seq_parameter_set_id;
}


int SpsParser::getChroma_format_idc() const {
  return chroma_format_idc;
}

int SpsParser::getSeparate_colour_plane_flag() const {
  return separate_colour_plane_flag;
}

SpsParser::~SpsParser() = default;