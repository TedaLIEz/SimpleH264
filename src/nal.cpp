//
// Created by aLIEzTed on 2/11/18.
//

#include "nal.h"
#include <iostream>
#include "io.h"
int find_nal_prefix(const std::string& file_name, std::vector<uint8_t> &nalBytes) {
  char* mem_block;
  if (!io::file_exist(file_name)) {
    std::cerr << "file not exist!" << std::endl;
    return -1;
  }
  std::ifstream file(file_name, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error opening file!" << std::endl;
    return -1;
  }
  auto size = file.tellg();
  mem_block = new char[size];
  file.seekg(0, std::ios::beg);
  file.read(mem_block, size);
  delete[] mem_block;
  file.close();
  std::cout << "Read all data in memory successfully" << std::endl;
  return 0;
}