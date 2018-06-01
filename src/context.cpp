//
// Created by aLIEzTed on 5/23/18.
//
#include "context.h"
#include <memory>
Context* Context::ctx = 0;

void Context::add_sps(SPS sps) {
  auto pair = std::make_pair(sps.seq_parameter_set_id, sps);
  sps_table.insert(pair);
}

const SPS& Context::lookup_sps_table(int id) {
  return sps_table[id];
}

const PPS& Context::lookup_pps_table(int id) {
  return pps_table[id];
}

void Context::add_pps(PPS pps) {
  auto pair = std::make_pair(pps.pic_parameter_set_id, pps);
  pps_table.insert(pair);
}

