#include <iostream>
#include "io.h"
#include "nalu.h"
#include "golomb.h"
int main() {
//  unsigned long size;
//  unsigned char *nalu;
  auto *uev = new u_char[2];
  long len = 0;
  auto rst = golomb::get_sev_encode(-15, uev, len);
  std::cout << "encode len " << len << std::endl;
  std::cout << "encode rst " << (int)uev[0] << std::endl;
  rst = golomb::get_sev_decode(uev, 0, 0);
  std::cout << "decode rst " << rst << std::endl;
//  std::ifstream file("sample/test_1080p.264", std::ios::binary);
//  if (!file.is_open()) {
//    return 0;
//  }
//  if (read_one_nalu(file, 0, nalu, size)) {
//    std::cout << "read data length " << size << std::endl;
//    for (int i = 0; i < size; i++) {
//      std::cout << "data " << std::hex << (unsigned int)*(nalu + i) << std::endl;
//    }
//  }
//
//  std::cout << "=====================" << std::endl << std::endl;
//  if (read_one_nalu(file, size + 4, nalu, size)) {
//    std::cout << "read data length " << size << std::endl;
//  }
//  delete nalu;
  return 0;
}