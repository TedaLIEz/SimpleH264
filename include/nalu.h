//
// Created by aLIEzTed on 2/11/18.
//

#ifndef SIMPLEH264_NAL_H
#define SIMPLEH264_NAL_H
#define NAL_HEADER_SHORT 0x000001
#define NAL_HEADER_LONG 0x00000001
#include <vector>
#include <fstream>
#include <ostream>
#pragma pack(push, 1)
class nalu_header {
 public:
  unsigned forbidden_zero_bit : 1;
  unsigned nal_ref_idc : 2;
  unsigned nal_unit_type : 5;
  friend std::ostream &operator<<(std::ostream &os, const nalu_header &header);
};
#pragma pack(pop)
#endif //SIMPLEH264_NAL_H
