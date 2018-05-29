//
// Created by aLIEzTed on 2/11/18.
//

#ifndef SIMPLEH264_NAL_H
#define SIMPLEH264_NAL_H
#define NAL_HEADER_SHORT 0x000001
#define NAL_HEADER_LONG 0x00000001
#define NAL_TYPE_UNSPECIFIED 0
#define NAL_TYPE_SLICE 1
#define NAL_TYPE_SLICE_A 2
#define NAL_TYPE_SLICE_B 3
#define NAL_TYPE_SLICE_C 4
#define NAL_TYPE_SLICE_IDR 5
#define NAL_TYPE_SEI 6
#define NAL_TYPE_SPS 7
#define NAL_TYPE_PPS 8
#define NAL_TYPE_PIC_DELIMITER 9
#define NAL_TYPE_END_OF_SEQ 10
#define NAL_TYPE_END_OF_STREAM 11
#define NAL_TYPE_FILTER_DATA 12
#include <vector>
#include <fstream>
#include <ostream>
#pragma pack(push, 1)
struct NALU_header {
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
};
#pragma pack(pop)

class NALU {
 public:
  NALU_header header;
  unsigned char* data;
  unsigned long size;

  friend std::ostream &operator<<(std::ostream &os, const NALU &nalu);

};
#endif //SIMPLEH264_NAL_H
