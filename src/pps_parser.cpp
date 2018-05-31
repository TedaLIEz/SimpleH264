//
// Created by aLIEzTed on 5/21/18.
//

#include "parser/pps_parser.h"

PPS Pps_Parser::parse(unsigned char *data, unsigned long len, unsigned long &offset) {
  PPS pps{};
  pps.pic_parameter_set_id = uev_decode(data, offset, "pic_parameter_set_id");
  pps.seq_parameter_set_id = uev_decode(data, offset, "seq_parameter_set_id");
  pps.entropy_coding_mode_flag = get_bool(data, offset);
  pps.pic_order_present_flag = get_bool(data, offset);
  pps.num_slice_groups_minus1 = uev_decode(data, offset, "num_slice_groups_minus1");
  if (pps.num_slice_groups_minus1 > 0) {
    pps.slice_group_map_type = uev_decode(data, offset, "slice_group_map_type");
    if (pps.slice_group_map_type == 0) {
      for (int i = 0; i <= pps.num_slice_groups_minus1; i++) {
        pps.run_length_minus1.push_back(uev_decode(data, offset, "run_length_minus1"));
      }
    } else if (pps.slice_group_map_type == 2) {
      for (int i = 0; i <= pps.num_slice_groups_minus1; i++) {
        pps.top_left.push_back(uev_decode(data, offset, "top_left"));
        pps.bottom_right.push_back(uev_decode(data, offset, "bottom_right"));
      }
    } else if (pps.slice_group_map_type == 3
        || pps.slice_group_map_type == 4
        || pps.slice_group_map_type == 5) {
      pps.slice_group_change_direction_flag = get_bool(data, offset);
      pps.slice_group_change_rate_minus1 = uev_decode(data, offset, "slice_group_change_rate_minus1");
    } else if (pps.slice_group_map_type == 6) {
      pps.pic_size_in_map_units_minus1 = uev_decode(data, offset, "pic_size_in_map_units_minus1");
      for (int i = 0; i <= pps.pic_size_in_map_units_minus1; i++) {
        pps.slice_group_id.push_back(uev_decode(data, offset, "slice_group_id"));
      }
    }


  }
  pps.num_ref_idx_l0_default_active_minus1 = uev_decode(data, offset, "num_ref_idx_l0_default_active_minus1");
  pps.num_ref_idx_l1_default_active_minus1 = uev_decode(data, offset, "num_ref_idx_l1_default_active_minus1");
  pps.weighted_pred_flag = get_bool(data, offset);
  pps.weighted_bipred_idc = bit::next_bit(data, len, 2, offset);
  offset += 2;
  pps.pic_init_qp_minus26 = sev_decode(data, offset, "pic_init_qp_minus26");
  pps.pic_init_qs_minus26 = sev_decode(data, offset, "pic_init_qs_minus26");
  pps.chroma_qp_index_offset = sev_decode(data, offset, "chroma_qp_index_offset");
  pps.deblocking_filter_variables_present_flag = get_bool(data, offset);
  pps.constrained_intra_pred_flag = get_bool(data, offset);
  pps.redundant_pic_cnt_present_flag = get_bool(data, offset);
  int trailing_len = more_rbsp_data(data, len, offset);
  if (!trailing_len) {
    // TODO: more_rbsp_data()
#ifdef DEBUG
    std::cout << "more_rbsp_data() in pps at offset " << offset << std::endl;
#endif
    pps.transform_8x8_mode_flag = get_bool(data, offset);
    pps.pic_scaling_matrix_present_flag = get_bool(data, offset);
  }
  offset += trailing_len;
  return pps;
}

int Pps_Parser::getType() {
  return id;
}

int Pps_Parser::more_rbsp_data(unsigned char* data, unsigned long len, unsigned long offset) {
  auto trailing_len = len * 8 - offset;
  auto trailing_rst = bit::next_bit(data, len, trailing_len, offset);
  auto mask = 1 << (trailing_len - 1);
  if (mask != trailing_rst) return 0;
  else return trailing_len;
}

Pps_Parser::~Pps_Parser() = default;