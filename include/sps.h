//
// Created by aLIEzTed on 5/14/18.
//

#ifndef SIMPLEH264_SPS_H
#define SIMPLEH264_SPS_H
#include "parser.h"
#include "bitutil.h"
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

 public:
  SpsParser() : Parser() {}
  void parse(unsigned char* data, unsigned long len) override;

  int getProfile_idc() const;

  int getLevel_idc() const;
  int getType() override;

  int getSeq_param_set_id() const;

  virtual ~SpsParser() override;
};
#endif //SIMPLEH264_SPS_H
