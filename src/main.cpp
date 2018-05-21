#include <iostream>
#include <parser/nalu_parser.h>
#include <util/io_h264.h>
#include <bitstream.h>
#include "util/debug.h"
#include "util/golomb.h"
#include "parser/sps_parser.h"
#include "parser/nalu_parser.h"
void test_nalu();

int main() {
  test_nalu();

  return 0;
}

void test_nalu() {
  auto stream = new BitStream();
  stream->start_parse("sample/test_1080p.264");

}