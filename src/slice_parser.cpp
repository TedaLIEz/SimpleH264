//
// Created by aLIEzTed on 5/30/18.
//

#include <parser/slice_parser.h>
#include <util/math.h>
#include <algorithm>

int Slice_Parser::NextMbAddress(int val, const Slice_header &hdr) {
  auto i = val + 1;
  auto ctx = Context::getInstance();
  auto pps = ctx->lookup_pps_table(hdr.pic_parameter_set_id);
  auto sps = ctx->lookup_sps_table(pps.seq_parameter_set_id);
  auto mapUnitToSliceGroupMap = mapUnitToSliceGroupMapGen(hdr);
  auto MbToSliceGroupMap = MbToSliceGroupMapGen(hdr, mapUnitToSliceGroupMap);
  auto PicHeightInMapUnits = sps.pic_height_in_map_units_minus1 + 1;
  auto FrameHeightInMbs = (2 - sps.frame_mbs_only_flag) * PicHeightInMapUnits;
  auto PicHeightInMbs = FrameHeightInMbs / (1 + hdr.field_pic_flag);
  auto PicWidthInMbs = sps.pic_width_in_mbs_minus1 + 1;
  auto PicSizeInMbs = PicHeightInMbs * PicWidthInMbs;
  while (i < PicSizeInMbs && MbToSliceGroupMap[i] != MbToSliceGroupMap[val]) {
    i++;
  }
  delete[] mapUnitToSliceGroupMap;
  delete[] MbToSliceGroupMap;
  return i;
}

int *Slice_Parser::MbToSliceGroupMapGen(const Slice_header &hdr, const int *mapUnitToSliceGroupMap) {
  auto ctx = Context::getInstance();
  auto pps = ctx->lookup_pps_table(hdr.pic_parameter_set_id);
  auto sps = ctx->lookup_sps_table(pps.seq_parameter_set_id);
  auto PicHeightInMapUnits = sps.pic_height_in_map_units_minus1 + 1;
  auto FrameHeightInMbs = (2 - sps.frame_mbs_only_flag) * PicHeightInMapUnits;
  auto PicHeightInMbs = FrameHeightInMbs / (1 + hdr.field_pic_flag);

  auto PicWidthInMbs = sps.pic_width_in_mbs_minus1 + 1;
  auto PicSizeInMbs = PicHeightInMbs * PicWidthInMbs;
  auto MbToSliceGroupMap = new int[PicSizeInMbs];
  auto MbaffFrameFlag = sps.mb_adaptive_frame_field_flag && !hdr.field_pic_flag;
  if (sps.frame_mbs_only_flag || hdr.field_pic_flag) {
    for (int i = 0; i < PicSizeInMbs; i++) {
      MbToSliceGroupMap[i] = mapUnitToSliceGroupMap[i];
    }
  } else if (MbaffFrameFlag) {
    for (int i = 0; i < PicSizeInMbs; i++) {
      MbToSliceGroupMap[i] = mapUnitToSliceGroupMap[i / 2];
    }
  } else {
    for (int i = 0; i < PicSizeInMbs; i++) {
      MbToSliceGroupMap[i] = mapUnitToSliceGroupMap[(i / (2 * PicWidthInMbs)) * PicWidthInMbs + (i % PicWidthInMbs)];
    }
  }
  return MbToSliceGroupMap;
}

int *Slice_Parser::mapUnitToSliceGroupMapGen(const Slice_header &hdr) {
  auto ctx = Context::getInstance();
  auto pps = ctx->lookup_pps_table(hdr.pic_parameter_set_id);
  auto sps = ctx->lookup_sps_table(pps.seq_parameter_set_id);
  auto PicHeightInMapUnits = sps.pic_height_in_map_units_minus1 + 1;

  auto PicWidthInMbs = sps.pic_width_in_mbs_minus1 + 1;

  auto PicSizeInMapUnits = PicWidthInMbs * PicHeightInMapUnits;

  auto mapUnitToSliceGroupMap = new int[PicSizeInMapUnits];
  if (pps.num_slice_groups_minus1 == 0) {
    memset(mapUnitToSliceGroupMap, 0, sizeof(int));
  } else {
    if (pps.slice_group_map_type == 0) {
      // specified in 8.2.4.1
      auto i = 0;
      do {
        for (auto iGroup = 0; iGroup <= pps.num_slice_groups_minus1 && i < PicSizeInMapUnits;
             i += pps.run_length_minus1[iGroup++] + 1) {
          for (auto j = 0; j <= pps.run_length_minus1[iGroup] && i + j < PicSizeInMapUnits; j++) {
            mapUnitToSliceGroupMap[i + j] = iGroup;
          }
        }
      } while (i < PicSizeInMapUnits);
    } else if (pps.slice_group_map_type == 1) {
      // specified in 8.2.4.2
      for (auto i = 0; i < PicSizeInMapUnits; i++) {
        mapUnitToSliceGroupMap[i] = (i % PicWidthInMbs +
            (i / PicWidthInMbs * (pps.num_slice_groups_minus1 + 1) / 2)) % (pps.num_slice_groups_minus1 + 1);

      }
    } else if (pps.slice_group_map_type == 2) {
      // specified in 8.2.4.3
      for (auto i = 0; i < PicSizeInMapUnits; i++) {
        mapUnitToSliceGroupMap[i] = pps.num_slice_groups_minus1;
      }

      for (auto iGroup = pps.num_slice_groups_minus1 - 1; iGroup >= 0; iGroup--) {
        auto yTopLeft = pps.top_left[iGroup] / PicWidthInMbs;
        auto xTopLeft = pps.top_left[iGroup] % PicWidthInMbs;
        auto yBottomRight = pps.bottom_right[iGroup] / PicWidthInMbs;
        auto xBottomRight = pps.bottom_right[iGroup] % PicWidthInMbs;
        for (auto y = yTopLeft; y <= yBottomRight; y++)
          for (auto x = xTopLeft; x <= xBottomRight; x++) {
            mapUnitToSliceGroupMap[y * PicWidthInMbs + x] = iGroup;
          }

      }
    } else if (pps.slice_group_map_type == 3) {
      // specified in 8.2.4.4
      ASSERT(pps.num_slice_groups_minus1 == 1,
             "num_slice_groups_minus1 in NextMbAddress of slice_group_map_type 3 failed");
      for (auto i = 0; i < PicSizeInMapUnits; i++) {
        mapUnitToSliceGroupMap[i] = 1;
      }
      auto SliceGroupChangeRate = pps.slice_group_change_rate_minus1 + 1;
      auto mapUnitsInSliceGroup0 = std::min(hdr.slice_group_change_cycle * SliceGroupChangeRate, PicSizeInMapUnits);
      auto x = (PicWidthInMbs - pps.slice_group_change_direction_flag) / 2;
      auto y = (PicHeightInMapUnits - pps.slice_group_change_direction_flag) / 2;
      auto leftBound = x, topBound = y;
      auto rightBound = x, bottomBound = y;
      auto xDir = pps.slice_group_change_direction_flag - 1;
      int yDir = pps.slice_group_change_direction_flag;
      auto mapUnitVacant = 0;
      for (auto k = 0; k < mapUnitsInSliceGroup0; k += mapUnitVacant) {
        mapUnitVacant = mapUnitToSliceGroupMap[y * PicWidthInMbs + x] == 1;
        if (mapUnitVacant) {
          mapUnitToSliceGroupMap[y * PicWidthInMbs + x] = 0;
        }

        if (xDir == -1 && x == leftBound) {
          leftBound = std::max(leftBound - 1, 0);
          x = leftBound;
          xDir = 0;
          yDir = 2 * pps.slice_group_change_direction_flag - 1;
        } else if (xDir == 1 && x == rightBound) {
          rightBound = std::min(rightBound + 1, PicWidthInMbs - 1);
          x = rightBound;
          xDir = 0;
          yDir = 1 - 2 * pps.slice_group_change_direction_flag;
        } else if (yDir == -1 && y == topBound) {
          topBound = std::max(topBound - 1, 0);
          y = topBound;
          xDir = 1 - 2 * pps.slice_group_change_direction_flag;
          yDir = 0;
        } else if (yDir == 1 && y == bottomBound) {
          bottomBound = std::min(bottomBound + 1, PicHeightInMapUnits - 1);
          y = bottomBound;
          xDir = 2 * pps.slice_group_change_direction_flag - 1;
          yDir = 0;
        } else {
          x = x + xDir;
          y = y + yDir;
        }
      }

    } else if (pps.slice_group_map_type == 4) {
      // specified in 8.2.4.5
      ASSERT(pps.num_slice_groups_minus1 == 1,
             "num_slice_groups_minus1 in NextMbAddress of slice_group_map_type 4 failed");
      auto SliceGroupChangeRate = pps.slice_group_change_rate_minus1 + 1;
      auto mapUnitsInSliceGroup0 = std::min(hdr.slice_group_change_cycle * SliceGroupChangeRate, PicSizeInMapUnits);
      auto sizeOfUpperLeftGroup = pps.slice_group_change_direction_flag ?
                                  (PicSizeInMapUnits - mapUnitsInSliceGroup0) : mapUnitsInSliceGroup0;
      for (auto j = 0; j < PicSizeInMapUnits; j++)
        if (j < sizeOfUpperLeftGroup) {
          mapUnitToSliceGroupMap[j] = pps.slice_group_change_direction_flag;
        } else {
          mapUnitToSliceGroupMap[j] = 1 - pps.slice_group_change_direction_flag;
        }
    } else if (pps.slice_group_map_type == 5) {
      // specified in 8.2.4.6
      ASSERT(pps.num_slice_groups_minus1 == 1,
             "num_slice_groups_minus1 in NextMbAddress of slice_group_map_type 4 failed");
      auto SliceGroupChangeRate = pps.slice_group_change_rate_minus1 + 1;
      auto mapUnitsInSliceGroup0 = std::min(hdr.slice_group_change_cycle * SliceGroupChangeRate, PicSizeInMapUnits);
      auto sizeOfUpperLeftGroup = pps.slice_group_change_direction_flag ?
                                  (PicSizeInMapUnits - mapUnitsInSliceGroup0) : mapUnitsInSliceGroup0;
      auto k = 0;
      for (auto j = 0; j < PicWidthInMbs; j++)
        for (auto i = 0; i < PicHeightInMapUnits; i++)
          if (k++ < sizeOfUpperLeftGroup) {
            mapUnitToSliceGroupMap[i * PicWidthInMbs + j] = pps.slice_group_change_direction_flag;
          } else {
            mapUnitToSliceGroupMap[i * PicWidthInMbs + j] = 1 - pps.slice_group_change_direction_flag;
          }
    } else if (pps.slice_group_map_type == 6) {
      // specified in 8.2.4.7
      for (auto i = 0; i < PicSizeInMapUnits; i++) {
        mapUnitToSliceGroupMap[i] = pps.slice_group_id[i];
      }

    } else {
#ifdef DEBUG
      std::cerr << "Invalid slice_group_map_type " << pps.slice_group_map_type << " in mapUnitToSliceGroupMap"
                << std::endl;
#endif
      ASSERT(false, "Invalid slice_group_map_type");
    }
  }
  return mapUnitToSliceGroupMap;
}

Slice Slice_Parser::parse(unsigned char *data, unsigned long len, unsigned long &offset) {
  Slice slice{};
  slice.hdr = parse_header(data, offset);
  auto ctx = Context::getInstance();
  auto pps = ctx->lookup_pps_table(slice.hdr.pic_parameter_set_id);
  auto sps = ctx->lookup_sps_table(pps.seq_parameter_set_id);
  if (pps.entropy_coding_mode_flag) {
    // aligned bits
    auto align = offset;
    while (offset % 8 != 0) {
      get_bool(data, offset);
    }
#ifdef DEBUG
    std::cout << "find aligned bits in " << (offset - align) << " bits" << std::endl;
#endif
  }
  auto MbaffFrameFlag = sps.mb_adaptive_frame_field_flag && !slice.hdr.field_pic_flag;
  auto CurrMbAddr = slice.hdr.first_mb_in_slice * (1 + MbaffFrameFlag);
  bool moreDataFlag(true);
  bool prevMbSkipped(false);
  do {
    if (slice.hdr.slice_type % 5 != SLICE_TYPE_I && slice.hdr.slice_type != SLICE_TYPE_SI) {
      if (!pps.entropy_coding_mode_flag) {
        slice.mb_skip_run = uev_decode(data, offset, "mb_skip_run");
        prevMbSkipped = slice.mb_skip_run > 0;
        for (int i = 0; i < slice.mb_skip_run; i++) {
          CurrMbAddr = NextMbAddress(CurrMbAddr, slice.hdr);
        }
        moreDataFlag = more_rbsp_data(data, len, offset) == 0;
      } else {
        // TODO: ae(v)
        slice.mb_skip_flag = true;
        moreDataFlag = !slice.mb_skip_flag;
      }
    }
  } while (moreDataFlag);
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

  if (hdr.slice_type % 5 == SLICE_TYPE_P || hdr.slice_type % 5 == SLICE_TYPE_SP
      || hdr.slice_type % 5 == SLICE_TYPE_B) {
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