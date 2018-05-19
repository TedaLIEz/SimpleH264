//
// Created by aLIEzTed on 2/11/18.
//

#ifndef SIMPLEH264_NAL_HEADER_H
#define SIMPLEH264_NAL_HEADER_H
#define NAL_HEADER_SHORT 0x000001
#define NAL_HEADER_LONG 0x00000001
#include <vector>
#include <fstream>
#include <ostream>
#pragma pack(push, 1)
class nalu_header {
 public:
#if defined(__LITTLE_ENDIAN)
  unsigned int nal_unit_type : 5;
  unsigned int nal_ref_idc : 2;
  unsigned int forbidden_zero_bit : 1;
#elif defined(__BIG_ENDIAN)
  unsigned int forbidden_zero_bit : 1;
  unsigned int nal_ref_idc : 2;
  unsigned int nal_unit_type : 5;
#else
  #error	"ByteOrder Error!"
#endif
  friend std::ostream &operator<<(std::ostream &os, const nalu_header &header);
};
#pragma pack(pop)
#endif //SIMPLEH264_NAL_H
