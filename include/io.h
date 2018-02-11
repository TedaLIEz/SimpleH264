//
// Created by aLIEzTed on 2/11/18.
//

#ifndef SIMPLEH264_UTIL_H
#define SIMPLEH264_UTIL_H

#include <string>
#include <fstream>
namespace io {
inline bool file_exist(const std::string& file_path) {
  std::ifstream file(file_path);
  return file.good();
}
}
#endif //SIMPLEH264_UTIL_H
