//
// Created by aLIEzTed on 5/23/18.
//

#include "gmock/gmock.h"
#include "parser/pps_parser.h"
#include "parser/sps_parser.h"
TEST(Single_Context_Test, Single_Context_Test) {
  auto pps_parser = new Pps_Parser();
  Sps sps;
  sps.seq_parameter_set_id = 42;
  pps_parser->ctx.add_sps(sps);
  auto sps_parser = new Sps_Parser();
  EXPECT_EQ(sps_parser->ctx.lookup_sps_table(sps.seq_parameter_set_id)->seq_parameter_set_id, 42);
}
