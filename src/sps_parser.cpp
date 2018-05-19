//
// Created by aLIEzTed on 5/14/18.
//
#include <golomb.h>
#include <sstream>
#include "debug.h"
#include "sps_parser.h"
Sps SpsParser::parse(unsigned char *data, unsigned long len) {
  Sps sps;
  unsigned long offset = 0;
  auto profile_idc = bit::read_bytes(data, len, offset);
  sps.profile_idc = profile_idc;
  offset += 8;
  auto flags = static_cast<uint8_t>(bit::next_bit(data, len, 8, offset));
  offset += 8;
  sps.cons_set0_flag = static_cast<bool>(bit::get_bit(flags, 0));
  sps.cons_set1_flag = static_cast<bool>(bit::get_bit(flags, 1));
  sps.cons_set2_flag = static_cast<bool>(bit::get_bit(flags, 2));
  sps.cons_set3_flag = static_cast<bool>(bit::get_bit(flags, 3));
  sps.cons_set4_flag = static_cast<bool>(bit::get_bit(flags, 4));
  sps.cons_set5_flag = static_cast<bool>(bit::get_bit(flags, 5));
  auto zero_bits_mask = (1 << 2) - 1;
  auto reserved_zero_bits = flags & zero_bits_mask;
  ASSERT(reserved_zero_bits == 0, "Error sps in reserved_zero_bits");
#ifdef DEBUG
  std::cout << "set0_flag " << sps.cons_set0_flag << std::endl;
  std::cout << "set1_flag " << sps.cons_set1_flag << std::endl;
  std::cout << "set2_flag " << sps.cons_set2_flag << std::endl;
  std::cout << "set3_flag " << sps.cons_set3_flag << std::endl;
  std::cout << "set4_flag " << sps.cons_set4_flag << std::endl;
  std::cout << "set5_flag " << sps.cons_set5_flag << std::endl;
#endif
  sps.level_idc = bit::read_bytes(data, len, offset);
  offset += 8;
  long seq_len = -1;
  sps.seq_parameter_set_id = golomb::get_uev_decode(data, offset, seq_len);
  ASSERT(seq_len > 0, "Error getting length of decode of the seq_param_set_id");
  offset += seq_len;
  if (profile_idc == 100 || profile_idc == 110 || profile_idc == 122 || profile_idc == 244 || profile_idc == 44
      || profile_idc == 83 || profile_idc == 86 || profile_idc == 118 || profile_idc == 128 || profile_idc == 138
      || profile_idc == 139 || profile_idc == 134 || profile_idc == 135) {

    long chroma_len = -1;
    sps.chroma_format_idc = golomb::get_uev_decode(data, offset, chroma_len);
    ASSERT(chroma_len > 0, "Error getting length of decode of the chroma_format_idc");
    offset += chroma_len;
#ifdef DEBUG
    std::cout << "profile_idc satisfied, find chroma_format_idc " << sps.chroma_format_idc << std::endl;
#endif
    if (sps.chroma_format_idc == 3) {
      sps.separate_colour_plane_flag = bit::get_bit(data, offset);
      ASSERT(sps.separate_colour_plane_flag == 0 || sps.separate_colour_plane_flag == 1, "Invalid separate_colour_plane_flag");
      offset += 1;
    }
    unsigned long scaling_list_flag_len = 12;
    if (sps.chroma_format_idc != 3) {
      scaling_list_flag_len = 8;
    }
    sps.seq_scaling_list_present_flag.resize(scaling_list_flag_len);
    long luma_minus8_len = -1;
    sps.bit_depth_luma_minus8 = golomb::get_uev_decode(data, offset, luma_minus8_len);
    ASSERT(chroma_len > 0, "Error getting length of decode of the bit_depth_luma_minus8");
    offset += luma_minus8_len;
    long chroma_minus8_len = -1;
    sps.bit_depth_chroma_minus8 = golomb::get_uev_decode(data, offset, chroma_minus8_len);
    ASSERT(chroma_minus8_len > 0, "Error getting length of decode of the bit_depth_chroma_minus8");
    offset += chroma_minus8_len;
    sps.qpprime_y_zero_transform_bypass_flag = static_cast<bool>(bit::get_bit(data, offset));
    offset += 1;
    sps.seq_scaling_matrix_present_flag = static_cast<bool>(bit::get_bit(data, offset));
    offset += 1;
    if (sps.seq_scaling_matrix_present_flag) {
      for (int i = 0; i < scaling_list_flag_len; i++) {
        sps.seq_scaling_list_present_flag[i] = static_cast<bool>(bit::get_bit(data, offset));
        offset += 1;
#ifdef DEBUG
        auto tmp_offset = offset;
#endif
        if (sps.seq_scaling_list_present_flag[i]) {
#ifdef DEBUG
          std::cout << "find seq_scaling_list_present_flag in index " << i << "at offset " << offset << std::endl;
#endif
          if (i < 6) {
            scaling_list(data, offset, sps.scaling_list_16[i], 16, sps.use_default_scaling_matrix_flag_16[i]);
          } else {
            scaling_list(data, offset, sps.scaling_list_64[i - 6], 64, sps.use_default_scaling_matrix_flag_64[i - 6]);
          }
        }
#ifdef DEBUG
        std::cout << "offset changed from " << tmp_offset << " to " << offset << std::endl;
#endif
      }
    }
    long log2_max_frame_num_minus4_len = 0;
    sps.log2_max_frame_num_minus4 = golomb::get_uev_decode(data, offset, log2_max_frame_num_minus4_len);
    ASSERT(log2_max_frame_num_minus4_len > 0, "Error getting length of decode of the log2_max_frame_num_minus4");
    offset += log2_max_frame_num_minus4_len;
    long pic_order_cnt_type_len = 0;
    sps.pic_order_cnt_type = golomb::get_uev_decode(data, offset, pic_order_cnt_type_len);
    ASSERT(pic_order_cnt_type_len > 0, "Error getting length of decode of the pic_order_cnt_type");
    offset += pic_order_cnt_type_len;
    if (sps.pic_order_cnt_type == 0) {
      long log2_max_pic_order_cnt_lsb_minus4_len = 0;
      sps.log2_max_pic_order_cnt_lsb_minus4 = golomb::get_uev_decode(data, offset, log2_max_pic_order_cnt_lsb_minus4_len);
      ASSERT(log2_max_pic_order_cnt_lsb_minus4_len > 0,
             "Error getting length of decode of the log2_max_pic_order_cnt_lsb_minus4");
      offset += log2_max_frame_num_minus4_len;
    } else if (sps.pic_order_cnt_type == 1) {
      sps.delta_pic_order_always_zero_flag = static_cast<bool>(bit::get_bit(data, offset));
      offset += 1;
      long offset_for_non_ref_pic_len = 0;
      sps.offset_for_non_ref_pic = golomb::get_sev_decode(data, offset, offset_for_non_ref_pic_len);
      ASSERT(offset_for_non_ref_pic_len > 0, "Error getting length of decode of the offset_for_non_ref_pic");
      offset += offset_for_non_ref_pic_len;
      long offset_for_top_to_bottom_field_len = 0;
      sps.offset_for_top_to_bottom_field = golomb::get_sev_decode(data, offset, offset_for_top_to_bottom_field_len);
      ASSERT(offset_for_top_to_bottom_field_len > 0,
             "Error getting length of decode of the offset_for_top_to_bottom_field");
      offset += offset_for_top_to_bottom_field_len;
      long num_ref_frames_in_pic_order_cnt_cycle_len = 0;
      sps.num_ref_frames_in_pic_order_cnt_cycle =
          golomb::get_uev_decode(data, offset, num_ref_frames_in_pic_order_cnt_cycle_len);
      ASSERT(num_ref_frames_in_pic_order_cnt_cycle_len > 0,
             "Error getting length of decode of the offset_for_top_to_bottom_field");
      offset += num_ref_frames_in_pic_order_cnt_cycle_len;
      sps.offset_for_ref_frame.resize(sps.num_ref_frames_in_pic_order_cnt_cycle);
      for (int i = 0; i < sps.num_ref_frames_in_pic_order_cnt_cycle; i++) {
        long offset_for_ref_frame_len = 0;
        auto _offset_for_ref_frame = golomb::get_sev_decode(data, offset, offset_for_ref_frame_len);
        std::ostringstream stringStream;
        stringStream << "Error getting length of decode of the _offset_for_ref_frame at " << i;
        ASSERT(offset_for_ref_frame_len > 0,
               stringStream.str());
        offset += offset_for_ref_frame_len;
        sps.offset_for_ref_frame.push_back(_offset_for_ref_frame);
      }
    }

  }
  return sps;
}

void SpsParser::scaling_list(unsigned char *data,
                             unsigned long &offset,
                             int &scalingList,
                             int sizeOfScalingList,
                             bool &useDefaultScalingMatrixFlag) {
  int lastScale = 8;
  int nextScale = 8;
  int delta_scale;
  for (int i = 0; i < sizeOfScalingList; i++) {
    if (nextScale != 0) {
      long tmp = 0;
      delta_scale = golomb::get_sev_decode(data, offset, tmp);
#ifdef DEBUG
      std::cout << "delta_scale " << delta_scale << " at offset " << offset << " len: " << tmp << std::endl;
#endif
      offset += tmp;
      nextScale = (lastScale + delta_scale + 256) % 256;
      useDefaultScalingMatrixFlag = (i == 0 && nextScale == 0);
      scalingList = (nextScale == 0) ? lastScale : nextScale;
      lastScale = scalingList;
    }
  }

}


int SpsParser::getType() {
  return id;
}

SpsParser::~SpsParser() = default;