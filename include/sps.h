//
// Created by aLIEzTed on 5/14/18.
//

#ifndef SIMPLEH264_SPS_H
#define SIMPLEH264_SPS_H
#include "parser.h"
#include "bitutil.h"
class SpsParser : public Parser {
 private:
  const int id = 7;
  int profile_idc = -1;
  int level_idc = -1;
  int seq_parameter_set_id = -1;
 public:
  SpsParser() : Parser() {}
  void parse(unsigned char* data, unsigned long len) override {
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
    }
    std::cout << "set0_flag " << cons_set0_flag << std::endl;
    std::cout << "set1_flag " << cons_set1_flag << std::endl;
    std::cout << "set2_flag " << cons_set2_flag << std::endl;
    std::cout << "set3_flag " << cons_set3_flag << std::endl;
    std::cout << "set4_flag " << cons_set4_flag << std::endl;
    std::cout << "set5_flag " << cons_set5_flag << std::endl;

  }

  int getProfile_idc() const {
    return profile_idc;
  }

  int getType() override {
    return id;
  }

};
#endif //SIMPLEH264_SPS_H
