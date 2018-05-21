#include <iostream>
#include <nalu_parser.h>
#include <io_h264.h>
#include <bitstream.h>
#include "debug.h"
#include "golomb.h"
#include "sps_parser.h"
#include "nalu_parser.h"
void test_nalu();

int main() {
  test_nalu();

  return 0;
}

void test_nalu() {
  auto stream = new BitStream();
  stream->start_parse("sample/test_1080p.264");

}