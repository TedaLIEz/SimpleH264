//
// Created by aLIEzTed on 5/14/18.
//
#include <golomb.h>
#include <sstream>
#include "debug.h"
#include "sps_parser.h"
#include "vui_parser.h"
Sps Sps_Parser::parse(unsigned char *data, unsigned long len, unsigned long &offset) {
  Sps sps;
//  unsigned long offset = 0;
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

  sps.seq_parameter_set_id = uev_decode(data, offset, "seq_parameter_set_id");
  if (profile_idc == 100 || profile_idc == 110 || profile_idc == 122 || profile_idc == 244 || profile_idc == 44
      || profile_idc == 83 || profile_idc == 86 || profile_idc == 118 || profile_idc == 128 || profile_idc == 138
      || profile_idc == 139 || profile_idc == 134 || profile_idc == 135) {

    sps.chroma_format_idc = uev_decode(data, offset, "chroma_format_idc");
#ifdef DEBUG
    std::cout << "profile_idc satisfied, find chroma_format_idc " << sps.chroma_format_idc << std::endl;
#endif
    if (sps.chroma_format_idc == 3) {
      sps.separate_colour_plane_flag = static_cast<bool>(bit::get_bit(data, offset));
      offset += 1;
    }

    sps.bit_depth_luma_minus8 = uev_decode(data, offset, "bit_depth_luma_minus8");
    sps.bit_depth_chroma_minus8 = uev_decode(data, offset, "bit_depth_chroma_minus8");
    sps.qpprime_y_zero_transform_bypass_flag = static_cast<bool>(bit::get_bit(data, offset));
    offset += 1;
    sps.seq_scaling_matrix_present_flag = static_cast<bool>(bit::get_bit(data, offset));
    offset += 1;
    if (sps.seq_scaling_matrix_present_flag) {
      unsigned long scaling_list_flag_len = 12;
      if (sps.chroma_format_idc != 3) {
        scaling_list_flag_len = 8;
      }
      sps.seq_scaling_list_present_flag.resize(scaling_list_flag_len);
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
  }

  sps.log2_max_frame_num_minus4 = uev_decode(data, offset, "log2_max_frame_num_minus4");
  sps.pic_order_cnt_type = uev_decode(data, offset, "pic_order_cnt_type");
  if (sps.pic_order_cnt_type == 0) {
    sps.log2_max_pic_order_cnt_lsb_minus4 = uev_decode(data, offset, "log2_max_pic_order_cnt_lsb_minus4");
  } else if (sps.pic_order_cnt_type == 1) {
    sps.delta_pic_order_always_zero_flag = static_cast<bool>(bit::get_bit(data, offset));
    offset += 1;

    sps.offset_for_non_ref_pic = sev_decode(data, offset, "offset_for_non_ref_pic");

    sps.offset_for_top_to_bottom_field = sev_decode(data, offset, "offset_for_top_to_bottom_field");

    sps.num_ref_frames_in_pic_order_cnt_cycle = uev_decode(data, offset, "num_ref_frames_in_pic_order_cnt_cycle");

    sps.offset_for_ref_frame.resize(sps.num_ref_frames_in_pic_order_cnt_cycle);
    for (int i = 0; i < sps.num_ref_frames_in_pic_order_cnt_cycle; i++) {
      long offset_for_ref_frame_len = -1;
      auto _offset_for_ref_frame = golomb::get_sev_decode(data, offset, offset_for_ref_frame_len);
      std::ostringstream stringStream;
      stringStream << "Error getting length of decode of the _offset_for_ref_frame at " << i;
      ASSERT(offset_for_ref_frame_len > 0,
             stringStream.str());
      offset += offset_for_ref_frame_len;
      sps.offset_for_ref_frame.push_back(_offset_for_ref_frame);
    }
  }

  sps.max_num_ref_frames = uev_decode(data, offset, "max_num_ref_frames");
  sps.gaps_in_frame_num_value_allowed_flag = static_cast<bool>(bit::get_bit(data, offset));
  offset += 1;

  sps.pic_width_in_mbs_minus1 = uev_decode(data, offset, "pic_width_in_mbs_minus1");
  sps.pic_height_in_map_units_minus1 = uev_decode(data, offset, "pic_height_in_map_units_minus1");

  sps.frame_mbs_only_flag = static_cast<bool>(bit::get_bit(data, offset));
  offset += 1;

  if (!sps.frame_mbs_only_flag) {
    sps.mb_adaptive_frame_field_flag = static_cast<bool>(bit::get_bit(data, offset));
    offset += 1;
  }

  sps.direct_8x8_inference_flag = static_cast<bool>(bit::get_bit(data, offset));
  offset += 1;

  sps.frame_cropping_flag = static_cast<bool>(bit::get_bit(data, offset));
  offset += 1;
  if (sps.frame_cropping_flag) {
    sps.frame_crop_left_offset = uev_decode(data, offset, "frame_crop_left_offset");
    sps.frame_crop_right_offset = uev_decode(data, offset, "frame_crop_right_offset");
    sps.frame_crop_top_offset = uev_decode(data, offset, "frame_crop_top_offset");
    sps.frame_crop_bottom_offset = uev_decode(data, offset, "frame_crop_bottom_offset");
  }

  sps.vui_parameters_present_flag = static_cast<bool>(bit::get_bit(data, offset));
  offset += 1;
  if (sps.vui_parameters_present_flag) {
    auto vui_parser = new VUI_Parser();
    sps.vui = vui_parser->parse(data, len, offset);
    delete vui_parser;
  }
  if (offset < len * 8) {
    auto trailing_bit = bit::get_bit(data, offset);
    ASSERT(trailing_bit == 1, "Fail to check the trailing bit");
    offset += 1;
#ifdef DEBUG
    std::cout << "trailing bit in sps, length= " << (len * 8 - offset) << std::endl;
#endif
    offset = len * 8;
  }
  return sps;
}

void Sps_Parser::scaling_list(unsigned char *data,
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

int Sps_Parser::getType() {
  return id;
}

Sps_Parser::~Sps_Parser() = default;