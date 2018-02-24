//
// Created by aLIEzTed on 2/11/18.
//

#include <sstream>
#include "nalu.h"

std::ostream &operator<<(std::ostream &os, const nalu_header &header) {
  os << "forbidden_zero_bit: " << header.forbidden_zero_bit << " nal_ref_idc: " << header.nal_ref_idc
     << " nal_unit_type: " << header.nal_unit_type;
  return os;
}
