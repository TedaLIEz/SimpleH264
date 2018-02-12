//
// Created by aLIEzTed on 2/11/18.
//

#ifndef SIMPLEH264_UTIL_H
#define SIMPLEH264_UTIL_H

#include <string>
#include <fstream>
#include <vector>
namespace io {
inline bool file_exist(const std::string& file_path) {
  std::ifstream file(file_path);
  return file.good();
}

inline bool read_file(const std::string& file_path, unsigned char*& data, long& file_size) {
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
  file_size = static_cast<long>(file.tellg());
  data = new unsigned char[file_size];
  file.seekg(0, std::ios::beg);
  file.read(reinterpret_cast<char *>(data), file_size);
  file.close();
  return true;
}
}
#endif //SIMPLEH264_UTIL_H
