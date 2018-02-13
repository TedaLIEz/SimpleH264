//
// Created by aLIEzTed on 2/11/18.
//

#include <sstream>
#include "nalu.h"
#include "bitutil.h"
int find_nal_prefix(char *bits, unsigned long bit_len, std::vector<unsigned long> &nalBytes) {
  int offset = 0;
  int count = 0;
  while (offset < bit_len) {
    while (bit::next_bit(bits, bit_len, 24, offset) != NAL_HEADER_SHORT
        && bit::next_bit(bits, bit_len, 32, offset) != NAL_HEADER_LONG) {
      offset += 8;
      // find leading_zero_8bits
    }
    if (bit::next_bit(bits, bit_len, 24, offset) != NAL_HEADER_SHORT) {
      offset += 8;
      // find zero byte
    }
    auto header = bit::next_bit(bits, bit_len, 24, offset);
    if (header != NAL_HEADER_SHORT) {
      std::cerr << "Invalid nalu " << std::endl;
      return -1;
    }
    // find start_code_prefix_one_3bytes
    // find nalu header
    offset += 24;
    int len = 0;
    int start_offset = offset;
    while (offset < bit_len && bit::next_bit(bits, bit_len, 24, offset) != NAL_HEADER_SHORT
        && bit::next_bit(bits, bit_len, 32, offset) != NAL_HEADER_LONG) {
      offset += 8;
      len += 8;
    }
    auto data = bit::next_bit(bits, bit_len, len, start_offset);
#ifdef DEBUG
    std::cout << "find nal, len " << len << std::endl;
    std::cout << "start_offset " << start_offset << std::endl;
    std::cout << "find data " << std::hex << data << std::endl;
#endif
    nalBytes.push_back(static_cast<unsigned long &&>(data));
    count++;
  }
  return count;
}

int read_one_nalu(std::ifstream& file, unsigned long start, unsigned char *&nalu, unsigned long &nalu_size) {
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
  while (bit::next_bit(buffer, 32, 24, 0) != NAL_HEADER_SHORT
      && bit::next_bit(buffer, 32, 32, 0) != NAL_HEADER_LONG) {
    std::cout << "find leading zero byte" << std::endl;
    memset(buffer, 0, 4);
    file.read(buffer, 4);
    offset += 1;
  }
  if (bit::next_bit(buffer, 32, 24, 0) != NAL_HEADER_SHORT) {
    offset += 1;
    // find zero byte
  }
  file.seekg(offset, std::ios::beg);
  file.read(header, 6);
  if (bit::next_bit(header, 24, 24, 0) != NAL_HEADER_SHORT) {
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
  while (offset < (long)file_size) {
    file.seekg(offset, std::ios::beg);
    if (offset < (long) file_size - 4) {
      memset(buffer, 0, 4);
      file.read(buffer, 4);
      if (bit::next_bit(buffer, 32, 24, 0) == NAL_HEADER_SHORT
          || bit::next_bit(buffer, 32, 32, 0) == NAL_HEADER_LONG) {
        break;
      }
      file.seekg((int)file.tellg() - 4, std::ios::beg);
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