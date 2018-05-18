//
// Created by aLIEzTed on 5/14/18.
//

#ifndef SIMPLEH264_SPS_H
#define SIMPLEH264_SPS_H
#include "parser.h"
#include "bitutil.h"
#include <vector>
#ifdef _TEST
#include <gtest/gtest_prod.h>
#include <vector>
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
  std::vector<bool> seq_scaling_list_present_flag;
  int scaling_list_16[16]{};
  int scaling_list_64[64]{};
  bool use_default_scaling_matrix_flag_16[16]{};
  bool use_default_scaling_matrix_flag_64[64]{};
  int log2_max_frame_num_minus4;
  int pic_order_cnt_type;
  int log2_max_pic_order_cnt_lsb_minus4;
#ifdef _TEST
  FRIEND_TEST(SpsTestCase, SPS_Depth_chroma);
  FRIEND_TEST(SpsTestCase, SPS_Qpprime_y_zero_transform_bypass_flag);
  FRIEND_TEST(SpsTestCase, SPS_Seq_scaling_matrix_present_flag);
  FRIEND_TEST(SpsTestCase, SPS_Depth_luma);
  FRIEND_TEST(SpsTestCase, SPS_Separate_colour_plane_flag);
  FRIEND_TEST(SpsTestCase, SPS_Scaling_list_size_Test);

  FRIEND_TEST(SpsScalingListTest, SPS_Scaling_list);
  FRIEND_TEST(SpsTestCase, SPS_Log2_max_frame_num_minus4);
  FRIEND_TEST(SpsTestCase, SPS_Pic_order_cnt_type);
  FRIEND_TEST(SpsTestCase, SPS_log2_max_pic_order_cnt_lsb_minus4);
#endif

  void scaling_list(unsigned char *data,
                   unsigned long& offset,
                   int& scalingList,
                   int sizeOfScalingList,
                   bool &useDefaultScalingMatrixFlag);
 public:
  SpsParser() : Parser(), qpprime_y_zero_transform_bypass_flag(false), seq_scaling_matrix_present_flag(false) {}
  void parse(unsigned char *data, unsigned long len) override;

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
