//
// Created by aLIEzTed on 5/30/18.
//

#ifndef SIMPLEH264_SLICE_PARSER_H
#define SIMPLEH264_SLICE_PARSER_H
#include <h264/slice.h>
#include "parser.h"
#ifdef _TEST
#include <gtest/gtest_prod.h>
#endif
class Slice_Parser : public Parser<Slice> {
 private:
#ifdef _TEST
  FRIEND_TEST(SliceHeaderTestCase, Parse_Header_Test);
#endif
  const int id = -1;

  void ref_pic_list_reordering(unsigned char *data, unsigned long &offset, Slice_header &hdr);

  void pred_weight_table(unsigned char *data, unsigned long &offset, Slice_header &hdr);

  void dec_ref_pic_marking(unsigned char *data, unsigned long &offset, Slice_header &hdr);
  Slice_header parse_header(unsigned char *data, unsigned long &offset);

  int NextMbAddress(int val, const Slice_header& hdr);

  int* mapUnitToSliceGroupMapGen(const Slice_header& hdr);
 public:
  Slice_Parser() : Parser() {}

  Slice parse(unsigned char *data, unsigned long len, unsigned long &offset) override ;


  int getType() override;

  ~Slice_Parser() override;
};
#endif //SIMPLEH264_SLICE_PARSER_H
