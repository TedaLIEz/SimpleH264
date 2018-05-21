//
// Created by aLIEzTed on 2/11/18.
//

#include <sstream>
#include "nalu.h"

std::ostream &operator<<(std::ostream &os, const NALU &nalu) {
  os << "forbidden_zero_bit: " << nalu.header.forbidden_zero_bit << " nal_ref_idc: " << nalu.header.nal_ref_idc
     << " nal_unit_type: " << nalu.header.nal_unit_type;
  return os;
}


