//
// Created by aLIEzTed on 2/11/18.
//

#ifndef SIMPLEH264_NAL_H
#define SIMPLEH264_NAL_H
#define NAL_HEADER_SHORT 0x000001
#define NAL_HEADER_LONG 0x00000001
#include <vector>
#include <fstream>

int find_nal_prefix(unsigned char* bits, unsigned long bit_len, std::vector<unsigned long> &nalBytes);
#endif //SIMPLEH264_NAL_H
