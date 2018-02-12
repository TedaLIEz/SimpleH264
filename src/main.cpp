#include <iostream>
#include "io.h"
int main() {
  long file_size;
  unsigned char *data;
  if (io::read_file("sample/test_1080p.264", data, file_size)) {
    std::cout << "file size " << file_size << std::endl;
    for (int i = 0; i < 10; i++) {
      std::cout << std::hex << +*(data + i) << std::endl;
    }
    delete[] data;
  }
  return 0;
}