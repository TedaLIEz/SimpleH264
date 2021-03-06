//
// Created by aLIEzTed on 5/19/18.
//

#include "parser/hrd_parser.h"

HRD HRD_Parser::parse(unsigned char *data, unsigned long len, unsigned long& offset) {
  HRD hrd{};
//  unsigned long offset = 0;
  hrd.cpb_cnt_minus1 = uev_decode(data, offset, "cpb_cnt_minus1");
  hrd.bit_rate_scale = read_bit(data, 4, offset);
//  offset += 4;
  hrd.cpb_size_scale = read_bit(data, 4, offset);
//  offset += 4;
  for (int i = 0; i <= hrd.cpb_cnt_minus1; i++) {
    hrd.bit_rate_value_minus1.push_back(uev_decode(data, offset, "bit_rate_value_minus1"));
    hrd.cpb_size_value_minus1.push_back(uev_decode(data, offset, "cpb_size_value_minus1"));
    auto cbr_flag = get_bool(data, offset);
    hrd.cbr_flag.push_back(cbr_flag);
  }

  hrd.initial_cpb_removal_delay_length_minus1 = read_bit(data, 5, offset);
//  offset += 5;
  hrd.cpb_removal_delay_length_minus1 = read_bit(data, 5, offset);
//  offset += 5;
  hrd.dpb_output_delay_length_minus1 = read_bit(data, 5, offset);
//  offset += 5;
  hrd.time_offset_length = read_bit(data, 5, offset);
//  offset += 5;
  return hrd;
}


int HRD_Parser::getType() {
  return 0;
}