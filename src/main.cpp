#include <iostream>
#include "io.h"
#include "nalu.h"
int main() {
  unsigned long size;
  unsigned char *nalu;
  std::ifstream file("sample/test_1080p.264", std::ios::binary);
  if (!file.is_open()) {
    return 0;
  }
  if (read_one_nalu(file, 0, nalu, size)) {
    std::cout << "read data length " << size << std::endl;
    for (int i = 0; i < size; i++) {
      std::cout << "data " << std::hex << (unsigned int)*(nalu + i) << std::endl;
    }
  }

  std::cout << "=====================" << std::endl << std::endl;
  if (read_one_nalu(file, size + 4, nalu, size)) {
    std::cout << "read data length " << size << std::endl;
  }
  delete nalu;
  return 0;
}