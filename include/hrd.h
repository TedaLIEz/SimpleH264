//
// Created by aLIEzTed on 5/19/18.
//

#ifndef SIMPLEH264_HRD_H
#define SIMPLEH264_HRD_H
#include <vector>
class HRD {
 public:
  int cpb_cnt_minus1;
  int bit_rate_scale;
  int cpb_size_scale;
  std::vector<int> bit_rate_value_minus1;
  std::vector<int> cpb_size_value_minus1;
  std::vector<bool> cbr_flag;

  int initial_cpb_removal_delay_length_minus1;
  int cpb_removal_delay_length_minus1;
  int dpb_output_delay_length_minus1;
  int time_offset_length;

};
#endif //SIMPLEH264_HRD_H
