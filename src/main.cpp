#include <iostream>
#include "bitutil.h"
int main() {
  auto * bits = new unsigned char[2];
  bits[0] = 255;
  bits[1] = 255;
  std::bitset<8> a((int)bits[0]);
  std::bitset<8> b((int)bits[1]);
  std::cout << a;
  std::cout << " " << b << std::endl;
  std::cout << std::bitset<12>(bit::next_bit(bits, 2, 11, 4)) << std::endl;
  return 0;
}