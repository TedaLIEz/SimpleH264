//
// Created by aLIEzTed on 5/23/18.
//

#include "gmock/gmock.h"
#include "parser/pps_parser.h"
#include "parser/sps_parser.h"
TEST(Context_Test, Single_Context_Test) {
  auto ctx1 = Context::getInstance();
  auto ctx2 = Context::getInstance();
  EXPECT_EQ(ctx1, ctx2);
}

TEST(Context_Test, Add_SPS_Test) {
  auto ctx = Context::getInstance();
  SPS sps;
  sps.seq_parameter_set_id = 42;
  ctx->add_sps(sps);
  EXPECT_EQ(ctx->lookup_sps_table(42).seq_parameter_set_id, sps.seq_parameter_set_id);
}

TEST(Context_Test, Add_PPS_Test) {
  auto ctx = Context::getInstance();
  PPS pps;
  pps.pic_parameter_set_id = 42;
  ctx->add_pps(pps);
  EXPECT_EQ(ctx->lookup_pps_table(42).pic_parameter_set_id, pps.pic_parameter_set_id);
}


