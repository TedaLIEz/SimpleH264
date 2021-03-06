//
// Created by aLIEzTed on 5/19/18.
//
#include <parser/hrd_parser.h>
#include "parser/vui_parser.h"
#include "h264/nalu.h"
VUI VUI_Parser::parse(unsigned char *data, unsigned long len, unsigned long &offset) {
  VUI vui{};
//  unsigned long offset = 0;
  vui.aspect_ratio_info_present_flag = get_bool(data, offset);
  if (vui.aspect_ratio_info_present_flag) {
    vui.aspect_ratio_idc = bit::read_bytes(data, offset);
    offset += 8;
    if (vui.aspect_ratio_idc == Extended_SAR) {
      vui.sar_height = bit::next_bit(data, 16, offset);
      offset += 16;
      vui.sar_width = bit::next_bit(data, 16, offset);
      offset += 16;
    }
  }

  vui.overscan_info_present_flag = get_bool(data, offset);
  if (vui.overscan_info_present_flag) {
    vui.overscan_appropriate_flag = get_bool(data, offset);
  }
  vui.video_signal_type_present_flag = get_bool(data, offset);
  if (vui.video_signal_type_present_flag) {
    vui.video_format = bit::next_bit(data, 3, offset);
    offset += 3;
    vui.video_full_range_flag = get_bool(data, offset);
    vui.colour_description_present_flag = get_bool(data, offset);
    if (vui.colour_description_present_flag) {
      vui.colour_primaries = read_bit(data, 8, offset);
//      offset += 8;
      vui.transfer_characteristics = read_bit(data, 8, offset);
//      offset += 8;
      vui.matrix_coefficients = read_bit(data, 8, offset);
//      offset += 8;
    }
  }
  vui.chroma_loc_info_present_flag = get_bool(data, offset);
  if (vui.chroma_loc_info_present_flag) {
    vui.chroma_sample_loc_type_top_field = uev_decode(data, offset, "chroma_sample_loc_type_top_field");
    vui.chroma_sample_loc_type_bottom_field = uev_decode(data, offset, "chroma_sample_loc_type_bottom_field");
  }
  vui.timing_info_present_flag = get_bool(data, offset);
  if (vui.timing_info_present_flag) {
    vui.num_units_in_tick = bit::next_bit(data, 32, offset);
    offset += 32;
    vui.time_scale = bit::next_bit(data, 32, offset);
    offset += 32;
    vui.fixed_frame_rate_flag = get_bool(data, offset);
  }

  vui.nal_hrd_parameters_present_flag = get_bool(data, offset);
  if (vui.nal_hrd_parameters_present_flag) {
    auto hrd_parser = new HRD_Parser();
    vui.nal_hrd_param = hrd_parser->parse(data, len, offset);
    delete hrd_parser;
  }
  vui.vcl_hrd_parameters_present_flag = get_bool(data, offset);
  if (vui.vcl_hrd_parameters_present_flag) {
    auto hrd_parser = new HRD_Parser();
    vui.vcl_hrd_param = hrd_parser->parse(data, len, offset);
    delete hrd_parser;
  }

  if (vui.nal_hrd_parameters_present_flag || vui.vcl_hrd_parameters_present_flag) {
    vui.low_delay_hrd_flag = get_bool(data, offset);
  }
  vui.pic_struct_present_flag = get_bool(data, offset);
  vui.bitstream_restriction_flag = get_bool(data, offset);
  if (vui.bitstream_restriction_flag) {
    vui.motion_vectors_over_pic_boundaries_flag = get_bool(data, offset);
    vui.max_bytes_per_pic_denom = uev_decode(data, offset, "max_bytes_per_pic_denom");
    vui.max_bits_per_mb_denom = uev_decode(data, offset, "max_bits_per_mb_denom");
    vui.log2_max_mv_length_horizontal = uev_decode(data, offset, "log2_max_mv_length_horizontal");
    vui.log2_max_mv_length_vertical = uev_decode(data, offset, "log2_max_mv_length_vertical");
    vui.max_num_reorder_frames = uev_decode(data, offset, "max_num_reorder_frames");
    vui.max_dec_frame_buffering = uev_decode(data, offset, "max_dec_frame_buffering");
  }
  return vui;
}

int VUI_Parser::getType() {
  // TODO: assign proper type
  return 0;
}
