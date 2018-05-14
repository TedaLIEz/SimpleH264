
#include "gtest/gtest.h"

using namespace testing;

int main(int argc, char **argv)
{
  // initialize
  ::testing::InitGoogleTest(&argc, argv);
  // run
  return RUN_ALL_TESTS();
}
