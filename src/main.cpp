#include <iostream>
#include <nalu_parser.h>
#include "debug.h"
#include "golomb.h"
#include "sps.h"
void test_nalu();

void test_golomb();
int main() {
  test_nalu();

  return 0;
}

void test_nalu() {
  unsigned long size;
  unsigned char *nalu;
  std::ifstream file("sample/test_1080p.264", std::ios::binary);
  if (!file.is_open()) {
    return;
  }
  unsigned long offset = 0;
  if (read_one_nalu(file, offset, nalu, size)) {
    std::cout << "read data length in bytes " << size << std::endl;
    read_one_sodb(nalu, size);
    for (int i = 0; i < size; i++) {
      PRINT_HEX((unsigned int)*(nalu + i));
    }
    std::cout << "sodb length in bytes " << size << std::endl;
    auto header = parse_header(nalu, size);
    std::cout << "header " << header << std::endl;
  }
  offset += size;
  offset += 4;
  if (read_one_nalu(file, offset, nalu, size)) {
    std::cout << "read another data length in bytes " << size << std::endl;
    read_one_sodb(nalu, size);
    for (int i = 0; i < size; i++) {
      PRINT_HEX((unsigned int)*(nalu + i));
    }
    std::cout << "sodb length in bytes " << size << std::endl;
    auto header = parse_header(nalu, size);
    std::cout << "header " << header << std::endl;
  }
  offset += size;
  offset += 3;
  if (read_one_nalu(file, offset, nalu, size)) {
    std::cout << "read another data length in bytes " << size << std::endl;
    read_one_sodb(nalu, size);
    for (int i = 0; i < size; i++) {
      PRINT_HEX((unsigned int)*(nalu + i));
    }
    std::cout << "sodb length in bytes " << size << std::endl;
    auto header = parse_header(nalu, size);
    std::cout << "header " << header << std::endl;
  }

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