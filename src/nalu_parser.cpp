//
// Created by aLIEzTed on 2/24/18.
//

#include <fstream>
#include <nalu.h>
#include <bitutil.h>
#include <cstring>
#include <nalu_parser.h>

NALU NAL_Parser::parse(unsigned char *data, unsigned long len, unsigned long &offset) {
  NALU nalu{};
  auto rst = sodb(data, len);
  ASSERT(!rst, "Fail to convert nalu data to sodb");
  offset = len * 8;
  nalu.header = parse_header(data, offset);
  nalu.data = data;
  nalu.size = len;
  return nalu;
}

NALU_header NAL_Parser::parse_header(unsigned char *data, unsigned long& offset) {
  NALU_header header{};
  memcpy(&header, data, sizeof(header));
  offset += 8;
  return header;
}

int NAL_Parser::sodb(unsigned char *nalu, unsigned long &size) {
  if (bit::next_bit(nalu, size, 1, 0) != 0) {
    return -1;
  }
  int index = 0;
  while (index < size * 8 - 16) {
    auto bits = bit::next_bit(nalu, size, 16, index);
    if (bits == 0x0000) {
      bits = bit::next_bit(nalu, size, 16, index + 16);
      if (bits == 0x0300 || bits == 0x0301 || bits == 0x0302 || bits == 0x0303) {
#ifdef DEBUG
        std::cout << "find a emulation prevention bytes at " << index << std::endl;
#endif
        auto start_bytes = (index + 16) / 8;
        // don't use memcpy here, as if the objects overlap, the behavior is undefined
        // ref: http://en.cppreference.com/w/c/string/byte/memcpy
        memmove(nalu + start_bytes, nalu + start_bytes + 1, size - start_bytes - 1);
        size -= 1;
        index += 24;
        continue;
      }
    }
    index += 4;
  }
  return 0;
}

int NAL_Parser::getType() {
  return 0;
}