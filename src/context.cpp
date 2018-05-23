//
// Created by aLIEzTed on 5/23/18.
//
#include "context.h"
#include <memory>
std::unique_ptr<Context> Context::ctx = 0;

void Context::add_sps(Sps sps) {
  auto pair = std::make_pair(sps.seq_parameter_set_id, sps);
  sps_table.insert(pair);
}

const Sps* Context::lookup_sps_table(int id) {
  return &sps_table[id];
}

