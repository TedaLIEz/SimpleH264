//
// Created by aLIEzTed on 5/23/18.
//

#include "gmock/gmock.h"
#include "parser/pps_parser.h"
#include "parser/sps_parser.h"
TEST(Context_Test, Single_Context_Test) {
  auto pps_parser = new Pps_Parser();
  auto sps_parser = new Sps_Parser();
  EXPECT_EQ(&sps_parser->ctx, &pps_parser->ctx);
  delete pps_parser;
  delete sps_parser;
}

TEST(Context_Test, Add_SPS_Test) {
  auto sps_parser = new Sps_Parser();
  SPS sps;
  sps.seq_parameter_set_id = 42;
  sps_parser->ctx.add_sps(&sps);
  EXPECT_EQ(sps_parser->ctx.lookup_sps_table(42)->seq_parameter_set_id, sps.seq_parameter_set_id);
  delete sps_parser;
}

TEST(Context_Test, Add_PPS_Test) {
  auto sps_parser = new Sps_Parser();
  PPS pps;
  pps.pic_parameter_set_id = 42;
  sps_parser->ctx.add_pps(&pps);
  EXPECT_EQ(sps_parser->ctx.lookup_pps_table(42)->pic_parameter_set_id, pps.pic_parameter_set_id);
}


