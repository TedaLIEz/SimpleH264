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
  nalu.header = parse_header(data, offset);
  nalu.data = data;
  nalu.size = len - 1;
  return nalu;
}

NALU_header NAL_Parser::parse_header(unsigned char *data, unsigned long& offset) {
  NALU_header header{};
  memcpy(&header, data, sizeof(header));
  offset += 8;
  return header;
}

int NAL_Parser::getType() {
  return 0;
}

int read_one_sodb(unsigned char *nalu, unsigned long &nalu_size) {
  if (bit::next_bit(nalu, nalu_size, 1, 0) != 0) {
    return -1;
  }
  int index = 0;
  while (index < nalu_size * 8 - 16) {
    auto bits = bit::next_bit(nalu, nalu_size, 16, index);
    if (bits == 0x0000) {
      bits = bit::next_bit(nalu, nalu_size, 16, index + 16);
      if (bits == 0x0300 || bits == 0x0301 || bits == 0x0302 || bits == 0x0303) {
#ifdef DEBUG
        std::cout << "find a emulation prevention bytes at " << index << std::endl;
#endif
        auto start_bytes = (index + 16) / 8;
        // don't use memcpy here, as if the objects overlap, the behavior is undefined
        // ref: http://en.cppreference.com/w/c/string/byte/memcpy
        memmove(nalu + start_bytes, nalu + start_bytes + 1, nalu_size - start_bytes - 1);
        nalu_size -= 1;
        index += 24;
        continue;
      }
    }
    index += 4;
  }
  return 0;
}

int read_one_nalu(std::ifstream &file, unsigned long start, unsigned char *&nalu, unsigned long &nalu_size) {
  auto offset = start;
  unsigned long len = 0;
  file.seekg(0, std::ios::end);
  auto file_size = file.tellg();
  file.seekg(start, std::ios::beg);
  if (start >= file_size) {
    std::cerr << "Out of boundary!!!" << std::endl;
    return -1;
  }
  auto buffer = new char[4];
  auto header = new char[6];
  memset(buffer, 0, 4);
  file.read(buffer, 4);
  while (bit::next_bit(reinterpret_cast<unsigned char *>(buffer), 32, 24, 0) != NAL_HEADER_SHORT
      && bit::next_bit(reinterpret_cast<unsigned char *>(buffer), 32, 32, 0) != NAL_HEADER_LONG) {
    std::cout << "find leading zero byte" << std::endl;
    memset(buffer, 0, 4);
    file.read(buffer, 4);
    offset += 1;
  }
  if (bit::next_bit(reinterpret_cast<unsigned char *>(buffer), 32, 24, 0) != NAL_HEADER_SHORT) {
    offset += 1;
    // find zero byte
  }
  file.seekg(offset, std::ios::beg);
  file.read(header, 6);
  if (bit::next_bit(reinterpret_cast<unsigned char *>(header), 24, 24, 0) != NAL_HEADER_SHORT) {
    std::cout << "Invalid nalu header " << std::endl;
#ifdef DEBUG
    std::cout << "read header " << std::hex << +*header << std::endl;
#endif
    delete[] header;
    return -1;
  }
  // find start_code_prefix_one_3bytes
  // find nalu header
  offset += 3;

  auto begin = offset;
  while (offset < (long) file_size) {
    file.seekg(offset, std::ios::beg);
    if (offset < (long) file_size - 4) {
      memset(buffer, 0, 4);
      file.read(buffer, 4);
      if (bit::next_bit(reinterpret_cast<unsigned char *>(buffer), 32, 24, 0) == NAL_HEADER_SHORT
          || bit::next_bit(reinterpret_cast<unsigned char *>(buffer), 32, 32, 0) == NAL_HEADER_LONG) {
        break;
      }
      file.seekg((int) file.tellg() - 4, std::ios::beg);
    }
    len += 1;
    offset += 1;
  }
  file.seekg(begin, std::ios::beg);
  nalu_size = len;
  nalu = new unsigned char[nalu_size];
  file.read(reinterpret_cast<char *>(nalu), nalu_size);
  delete[] header;
  delete[] buffer;
  return 1;

}