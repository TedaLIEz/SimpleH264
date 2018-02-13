#include <iostream>
#include "io.h"
#include "nalu.h"
int main() {
  unsigned long file_size;
  unsigned char *data;
  std::vector<unsigned long> nalus;
  if (io::read_file("sample/test.h264", data, file_size)) {
    std::cout << "file size " << file_size << std::endl;
    auto count = find_nal_prefix(data, file_size * 8, nalus);
    if (count >= 0) {
      std::cout << "nalu count " << count << std::endl;
      for (auto &it : nalus) {
        std::cout << std::hex << it << std::endl;
      }
    }
    delete[] data;
  }
  return 0;
}