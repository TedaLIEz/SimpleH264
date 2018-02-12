#include <iostream>
#include "bitutil.h"
int main() {
  auto * bits = new unsigned char[3];
  bits[0] = 170;
  bits[1] = 85;
  bits[2] = 255;
  std::bitset<8> a((int)bits[0]);
  std::bitset<8> b((int)bits[1]);
  std::bitset<8> c((int)bits[2]);
  std::cout << a;
  std::cout << b;
  std::cout << c << std::endl;
  std::cout << std::bitset<13>(bit::next_bit(bits, 3, 13, 5)) << std::endl;
  return 0;
}