#include <iostream>
#include <nalu_parser.h>
#include "debug.h"
#include "golomb.h"
#include "sps_parser.h"
#include "nalu_parser.h"
void test_nalu();

void test_golomb();
int main() {
  test_nalu();

  return 0;
}

void test_nalu() {
  unsigned long size;
  unsigned char *data;
  std::ifstream file("sample/test_1080p.264", std::ios::binary);
  if (!file.is_open()) {
    return;
  }
  unsigned long offset = 0;
  auto parser = new NAL_Parser();
  if (read_one_nalu(file, offset, data, size)) {
    unsigned long nalu_offset = 0;
    auto nalu = parser->parse(data, size, nalu_offset);
    std::cout << "nalu size : " << nalu.size << nalu << std::endl;
    for (int i = 0; i < nalu.size; i++) {
      PRINT_HEX((unsigned int)*(nalu.data + i));
    }
  }
  offset += size;
  offset += 4;
  if (read_one_nalu(file, offset, data, size)) {
    unsigned long nalu_offset = 0;
    auto nalu = parser->parse(data, size, nalu_offset);
    std::cout << "nalu size : " << nalu.size << nalu << std::endl;
    for (int i = 0; i < nalu.size; i++) {
      PRINT_HEX((unsigned int)*(nalu.data + i));
    }
  }
  offset += size;
  offset += 3;
  if (read_one_nalu(file, offset, data, size)) {
    unsigned long nalu_offset = 0;
    auto nalu = parser->parse(data, size, nalu_offset);
    std::cout << "nalu size : " << nalu.size << nalu << std::endl;
    for (int i = 0; i < nalu.size; i++) {
      PRINT_HEX((unsigned int)*(nalu.data + i));
    }
  }
  delete parser;
}

void test_golomb() {
  auto *uev = new u_char[2];
  long len = 0;
  auto rst = golomb::get_sev_encode(-15, uev, len);
  std::cout << "encode len " << len << std::endl;
  std::cout << "encode rst " << (int)uev[0] << std::endl;
  rst = golomb::get_sev_decode(uev, 0, 0, len);
  std::cout << "decode rst " << rst << std::endl;
}