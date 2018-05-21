//
// Created by aLIEzTed on 2/11/18.
//

#ifndef SIMPLEH264_UTIL_H
#define SIMPLEH264_UTIL_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "bitutil.h"
#include "h264/nalu.h"
namespace io {
inline bool file_exist(const std::string& file_path) {
  std::ifstream file(file_path);
  return file.good();
}

inline bool read_file(const std::string& file_path, unsigned char*& data, unsigned long& file_size) {
  if (!file_exist(file_path)) {
    std::cerr << "file not exist!" << std::endl;
    return false;
  }
  std::ifstream file(file_path, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error opening file!" << std::endl;
    return false;
  }
  file.seekg(0, std::ios::end);
  file_size = static_cast<unsigned long>(file.tellg());
  data = new unsigned char[file_size];
  file.seekg(0, std::ios::beg);
  file.read(reinterpret_cast<char *>(data), file_size);
  file.close();
  return true;
}

inline int read_one_nalu(std::ifstream &file, unsigned long start, unsigned char *&nalu, unsigned long &nalu_size) {
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
}
#endif //SIMPLEH264_UTIL_H
