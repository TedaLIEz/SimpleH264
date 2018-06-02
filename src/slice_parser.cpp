//
// Created by aLIEzTed on 5/30/18.
//

#include <parser/slice_parser.h>
#include <util/math.h>

Slice Slice_Parser::parse(unsigned char *data, unsigned long len, unsigned long &offset) {
  Slice slice{};


  return slice;
}
Slice_header Slice_Parser::parse_header(unsigned char *data, unsigned long &offset) {
  Slice_header hdr{};
  offset += 8;   // skip the nal header in data
  hdr.first_mb_in_slice = uev_decode(data, offset, "first_mb_in_slice");
  hdr.slice_type = uev_decode(data, offset, "slice_type");
  hdr.pic_parameter_set_id = uev_decode(data, offset, "pic_parameter_set_id");
  auto ctx = Context::getInstance();
  auto pps = ctx->lookup_pps_table(hdr.pic_parameter_set_id);
  auto sps = ctx->lookup_sps_table(pps.seq_parameter_set_id);
  auto frame_num_len = sps.log2_max_frame_num_minus4 + 4;
  hdr.frame_num = read_bit(data, frame_num_len, offset);
//  offset += frame_num_len;
  // TODO: how to test this function with context
  if (!sps.frame_mbs_only_flag) {
    hdr.field_pic_flag = get_bool(data, offset);
    if (hdr.field_pic_flag) {
      hdr.bottom_field_flag = get_bool(data, offset);
    }
  }
  if ((data[0] & 0x0F) == 5) {
#ifdef DEBUG
    std::cout << "slice_header found nal type: 5" << std::endl;
#endif
    hdr.idr_pic_id = uev_decode(data, offset, "idr_pic_id");
  }
  if (sps.pic_order_cnt_type == 0) {
    auto pic_order_cnt_lsb_len = sps.log2_max_pic_order_cnt_lsb_minus4 + 4;
    hdr.pic_order_cnt_lsb = read_bit(data, pic_order_cnt_lsb_len, offset);
//    offset += pic_order_cnt_lsb_len;
    if (pps.pic_order_present_flag == 1 && !hdr.field_pic_flag) {
      hdr.delta_pic_order_cnt_bottom = sev_decode(data, offset, "delta_pic_order_cnt_bottom");
    }
  }

  if (sps.pic_order_cnt_type == 1 && !sps.delta_pic_order_always_zero_flag) {
    hdr.delta_pic_order_cnt[0] = sev_decode(data, offset, "delta_pic_order_cnt[0]");
    if (pps.pic_order_present_flag == 1 && !hdr.field_pic_flag) {
      hdr.delta_pic_order_cnt[1] = sev_decode(data, offset, "delta_pic_order_cnt[1]");
    }
  }

  if (pps.redundant_pic_cnt_present_flag) {
    hdr.redundant_pic_cnt = uev_decode(data, offset, "redundant_pic_cnt");
  }

  if (hdr.slice_type % 5 == SLICE_TYPE_B) {
    hdr.direct_spatial_mv_pred_flag = get_bool(data, offset);
  }

  if (hdr.slice_type % 5 == SLICE_TYPE_P || hdr.slice_type % 5 == SLICE_TYPE_SP || hdr.slice_type % 5 == SLICE_TYPE_B) {
    hdr.num_ref_idx_active_override_flag = get_bool(data, offset);
    if (hdr.num_ref_idx_active_override_flag) {
      hdr.num_ref_idx_l0_active_minus1 = uev_decode(data, offset, "num_ref_idx_l0_active_minus1");
      if (hdr.slice_type % 5 == SLICE_TYPE_B) {
        hdr.num_ref_idx_l1_active_minus1 = uev_decode(data, offset, "num_ref_idx_l1_active_minus1");
      }
    }
  }

  ref_pic_list_reordering(data, offset, hdr);
  if ((pps.weighted_pred_flag && (hdr.slice_type % 5 == SLICE_TYPE_P || hdr.slice_type % 5 == SLICE_TYPE_SP))
      || (pps.weighted_bipred_idc == 1 && hdr.slice_type % 5 == SLICE_TYPE_B)) {
#ifdef DEBUG
    std::cout << "find pred weight table in slice header at offset " << offset << std::endl;
#endif
    pred_weight_table(data, offset, hdr);
  }

  if ((data[0] & 0x60) != 0) {
    std::cout << "find dec_ref_pic_marking in slice header at offset " << offset << std::endl;
    dec_ref_pic_marking(data, offset, hdr);
  }

  if (pps.entropy_coding_mode_flag && hdr.slice_type % 5 != SLICE_TYPE_I && hdr.slice_type % 5 != SLICE_TYPE_SI) {
    hdr.cabac_init_idc = uev_decode(data, offset, "cabac_init_idc");
  }
  hdr.slice_qp_delta = sev_decode(data, offset, "slice_qp_delta");
  if (hdr.slice_type % 5 == SLICE_TYPE_SP || hdr.slice_type % 5 == SLICE_TYPE_SI) {
    if (hdr.slice_type % 5 == SLICE_TYPE_SP) {
      hdr.sp_for_switch_flag = get_bool(data, offset);
    }
    hdr.slice_qs_delta = sev_decode(data, offset, "slice_qs_delta");
  }

  if (pps.deblocking_filter_variables_present_flag) {
    hdr.disable_deblocking_filter_idc = uev_decode(data, offset, "disable_deblocking_filter_idc");
    if (hdr.disable_deblocking_filter_idc != 1) {
      hdr.slice_alpha_c0_offset_div2 = sev_decode(data, offset, "slice_alpha_c0_offset_div2");
      hdr.slice_beta_offset_div2 = sev_decode(data, offset, "slice_beta_offset_div2");
    }
  }
  if (pps.num_slice_groups_minus1 > 0 && pps.slice_group_map_type >= 3 && pps.slice_group_map_type <= 5) {
    auto slice_group_change_cycle_len = (sps.pic_height_in_map_units_minus1 + 1) * (sps.pic_width_in_mbs_minus1 + 1) /
        (pps.slice_group_change_rate_minus1 + 1);
    if (((sps.pic_height_in_map_units_minus1 + 1) * (sps.pic_width_in_mbs_minus1 + 1)) %
        (pps.slice_group_change_rate_minus1 + 1)) {
      slice_group_change_cycle_len += 1;
    }

    slice_group_change_cycle_len = math::CeilLog2(slice_group_change_cycle_len + 1);
    hdr.slice_group_change_cycle = read_bit(data, slice_group_change_cycle_len, offset);
//    offset += slice_group_change_cycle_len;
  }
  return hdr;
}

void Slice_Parser::ref_pic_list_reordering(unsigned char *data,
                                           unsigned long &offset,
                                           Slice_header &hdr) {
  if (hdr.slice_type % 5 != SLICE_TYPE_I && hdr.slice_type % 5 != SLICE_TYPE_SI) {
    hdr.ref_pic_list_reordering_flag_l0 = get_bool(data, offset);
    if (hdr.ref_pic_list_reordering_flag_l0) {
      do {
        hdr.reordering_of_pic_nums_idc = uev_decode(data, offset, "reordering_of_pic_nums_idc");
        if (hdr.reordering_of_pic_nums_idc == 0 || hdr.reordering_of_pic_nums_idc == 1) {
          hdr.abs_diff_pic_num_minus1 = uev_decode(data, offset, "abs_diff_pic_num_minus1");
        } else if (hdr.reordering_of_pic_nums_idc == 2) {
          hdr.long_term_pic_num = uev_decode(data, offset, "long_term_pic_num");
        }
      } while (hdr.reordering_of_pic_nums_idc != 3);
    }
  }

  if (hdr.slice_type % 5 == SLICE_TYPE_B) {
    hdr.ref_pic_list_reordering_flag_l1 = get_bool(data, offset);
    if (hdr.ref_pic_list_reordering_flag_l1) {
      do {
        hdr.reordering_of_pic_nums_idc = uev_decode(data, offset, "reordering_of_pic_nums_idc");
        if (hdr.reordering_of_pic_nums_idc == 0 || hdr.reordering_of_pic_nums_idc == 1) {
          hdr.abs_diff_pic_num_minus1 = uev_decode(data, offset, "abs_diff_pic_num_minus1");
        } else if (hdr.reordering_of_pic_nums_idc == 2) {
          hdr.long_term_pic_num = uev_decode(data, offset, "long_term_pic_num");
        }
      } while (hdr.reordering_of_pic_nums_idc != 3);
    }
  }
}

void Slice_Parser::pred_weight_table(unsigned char *data, unsigned long &offset, Slice_header &hdr) {
  hdr.luma_log2_weight_denom = uev_decode(data, offset, "luma_log2_weight_denom");
  hdr.chroma_log2_weight_denom = uev_decode(data, offset, "luma_log2_weight_denom");
  auto ctx = Context::getInstance();
  auto pps = ctx->lookup_pps_table(hdr.pic_parameter_set_id);
  hdr.chroma_weight_l0.reserve(pps.num_ref_idx_l0_default_active_minus1 + 1);
  hdr.chroma_offset_l0.reserve(pps.num_ref_idx_l0_default_active_minus1 + 1);
  for (int i = 0; i <= pps.num_ref_idx_l0_default_active_minus1; i++) {
    hdr.luma_weight_l0_flag = get_bool(data, offset);
    if (hdr.luma_weight_l0_flag) {
      hdr.luma_weight_l0.push_back(sev_decode(data, offset, "luma_weight_l0[i]"));
      hdr.luma_offset_l0.push_back(sev_decode(data, offset, "luma_offset_l0[i]"));
    }
    hdr.chroma_weight_l0_flag = get_bool(data, offset);
    if (hdr.chroma_weight_l0_flag) {
      hdr.chroma_weight_l0[i] = std::vector<int>(2);
      hdr.chroma_offset_l0[i] = std::vector<int>(2);
      for (int j = 0; j < 2; j++) {
        hdr.chroma_weight_l0[i].push_back(sev_decode(data, offset, "chroma_weight_l0[i][j]"));
        hdr.chroma_offset_l0[i].push_back(sev_decode(data, offset, "chroma_offset_l0[i][j]"));
      }
    }
  }
  if (hdr.slice_type == SLICE_TYPE_B) {
    hdr.chroma_weight_l1.reserve(pps.num_ref_idx_l1_default_active_minus1 + 1);
    hdr.chroma_offset_l1.reserve(pps.num_ref_idx_l1_default_active_minus1 + 1);
    for (int i = 0; i <= pps.num_ref_idx_l1_default_active_minus1; i++) {
      hdr.luma_weight_l1_flag = get_bool(data, offset);
      if (hdr.luma_weight_l1_flag) {
        hdr.luma_weight_l1.push_back(sev_decode(data, offset, "luma_weight_l0[i]"));
        hdr.luma_offset_l1.push_back(sev_decode(data, offset, "luma_offset_l0[i]"));
      }
      hdr.chroma_weight_l1_flag = get_bool(data, offset);
      if (hdr.chroma_weight_l1_flag) {
        hdr.chroma_weight_l1[i] = std::vector<int>(2);
        hdr.chroma_offset_l1[i] = std::vector<int>(2);
        for (int j = 0; j < 2; j++) {
          hdr.chroma_weight_l1[i].push_back(sev_decode(data, offset, "chroma_weight_l0[i][j]"));
          hdr.chroma_offset_l1[i].push_back(sev_decode(data, offset, "chroma_offset_l0[i][j]"));
        }
      }
    }
  }
}

void Slice_Parser::dec_ref_pic_marking(unsigned char *data, unsigned long &offset, Slice_header &hdr) {
  if ((data[0] & 0x0F) == 5) {
    hdr.no_output_of_prior_pics_flag = get_bool(data, offset);
    hdr.long_term_reference_flag = get_bool(data, offset);
  } else {
    hdr.adaptive_ref_pic_marking_mode_flag = get_bool(data, offset);
    if (hdr.adaptive_ref_pic_marking_mode_flag) {
      do {
        hdr.memory_management_control_operation = uev_decode(data, offset, "memory_management_control_operation");
        if (hdr.memory_management_control_operation == 1 || hdr.memory_management_control_operation == 3) {
          hdr.difference_of_pic_nums_minus1 = uev_decode(data, offset, "difference_of_pic_nums_minus1");
        }
        if (hdr.memory_management_control_operation == 2) {
          hdr.long_term_pic_num = uev_decode(data, offset, "long_term_pic_num");
        }
        if (hdr.memory_management_control_operation == 3 || hdr.memory_management_control_operation == 6) {
          hdr.long_term_frame_idx = uev_decode(data, offset, "long_term_frame_idx");
        }
        if (hdr.memory_management_control_operation == 4) {
          hdr.max_long_term_frame_idx_plus1 = uev_decode(data, offset, "max_long_term_frame_idx_plus1");
        }
      } while (hdr.memory_management_control_operation != 0);
    }
  }
}

int Slice_Parser::getType() {
  return id;
}
Slice_Parser::~Slice_Parser() = default;