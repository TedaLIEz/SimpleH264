//
// Created by aLIEzTed on 2/11/18.
//

#ifndef SIMPLEH264_NAL_H
#define SIMPLEH264_NAL_H

#include <vector>
#include <fstream>

int find_nal_prefix(const std::string& file_name, std::vector<uint8_t> &nalBytes);
#endif //SIMPLEH264_NAL_H
