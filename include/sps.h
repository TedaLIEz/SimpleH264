//
// Created by aLIEzTed on 5/14/18.
//

#ifndef SIMPLEH264_SPS_H
#define SIMPLEH264_SPS_H
#include "parser.h"
#include "bitutil.h"
#ifdef _TEST
#include <gtest/gtest_prod.h>
#endif
class SpsParser : public Parser {
 private:
  const int id = 7;
  int profile_idc = -1;
  int level_idc = -1;
  int seq_parameter_set_id = -1;
  int chroma_format_idc = -1;
  int separate_colour_plane_flag = -1;
  int bit_depth_luma_minus8 = -1;
  int bit_depth_chroma_minus8 = -1;
  bool qpprime_y_zero_transform_bypass_flag;
  bool seq_scaling_matrix_present_flag;
#ifdef _TEST
  FRIEND_TEST(SpsTest, SPS_Depth_chroma);
  FRIEND_TEST(SpsTest, SPS_Qpprime_y_zero_transform_bypass_flag);
  FRIEND_TEST(SpsTest, SPS_Seq_scaling_matrix_present_flag);
  FRIEND_TEST(SpsTest, SPS_Depth_luma);
  FRIEND_TEST(SpsTest, SPS_Separate_colour_plane_flag);
#endif
 public:
  SpsParser() : Parser() {}
  void parse(unsigned char* data, unsigned long len) override;

  int getProfile_idc() const;

  int getLevel_idc() const;
  int getType() override;

  int getSeq_param_set_id() const;

  int getChroma_format_idc() const;

  int getSeparate_colour_plane_flag() const;

  int getBit_depth_luma_minus8() const;

  int getBit_depth_chroma_minus8() const;

  bool isQpprime_y_zero_transform_bypass_flag() const;

  bool isSeq_scaling_matrix_present_flag() const;
  virtual ~SpsParser() override;
};
#endif //SIMPLEH264_SPS_H
