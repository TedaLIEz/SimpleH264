//
// Created by aLIEzTed on 5/21/18.
//

#include "bitstream.h"
#include <iostream>
#include <fstream>
#include <util/debug.h>
#include <util/io_h264.h>
// TODO: can this be unit tested?
void BitStream::start_parse(const std::string &str) {
  unsigned char *data;
  size_t size = 0;
  std::ifstream file(str, std::ios::binary);
  ASSERT(file.is_open(), "FILE FAIL TO OPEN");
  for (int i = 0; i < 3; i++) {
    if (!read_one_nalu(file, offset, data, size)) {
#ifdef DEBUG
      std::cout << "read one nalu, current offset " << offset << std::endl;
#endif
      unsigned long nalu_offset = 0;
      auto nalu = parser.parse(data, size, nalu_offset);
#ifdef DEBUG
      std::cout << "nalu size : " << nalu.size << nalu << std::endl;
      for (int j = 0; j < nalu.size; j++) {
        PRINT_HEX((unsigned int)*(nalu.data + j));
      }
#endif
    } else {
#ifdef DEBUG
      std::cout << "Fail to read one nalu at offset " << offset << std::endl;
#endif
      return;
    }
  }
  file.close();
}

int BitStream::read_one_nalu(std::ifstream &file,
                             unsigned long &start_in_bit,
                             unsigned char *&nalu,
                             unsigned long &nalu_size) {
  auto start = start_in_bit / 8;
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
  while (bit::next_bit(reinterpret_cast<unsigned char *>(buffer), 24, 0) != NAL_HEADER_SHORT
      && bit::next_bit(reinterpret_cast<unsigned char *>(buffer), 32, 0) != NAL_HEADER_LONG) {
    std::cout << "find leading zero byte" << std::endl;
    memset(buffer, 0, 4);
    file.read(buffer, 4);
    offset += 1;
  }
  if (bit::next_bit(reinterpret_cast<unsigned char *>(buffer), 24, 0) != NAL_HEADER_SHORT) {
    offset += 1;
    // find zero byte
  }
  file.seekg(offset, std::ios::beg);
  file.read(header, 6);
  if (bit::next_bit(reinterpret_cast<unsigned char *>(header), 24, 0) != NAL_HEADER_SHORT) {
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
      if (bit::next_bit(reinterpret_cast<unsigned char *>(buffer), 24, 0) == NAL_HEADER_SHORT
          || bit::next_bit(reinterpret_cast<unsigned char *>(buffer), 32, 0) == NAL_HEADER_LONG) {
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
  start_in_bit = offset * 8;
  return 0;
}