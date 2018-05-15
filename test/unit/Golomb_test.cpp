//
// Created by aLIEzTed on 5/15/18.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "golomb.h"
#include <cstring>
TEST(Golomb, Golomb_uev_encode_test) {
  auto * bits = new unsigned char;
  long len = -1;
  golomb::get_uev_encode(0, bits, len);
  EXPECT_EQ(len, 1);
  unsigned char rst1[] = {0x80};
  EXPECT_EQ(memcmp(bits, &rst1, len / 8 + 1), 0);
  memset(bits, 0, len / 8 + 1);

  golomb::get_uev_encode(1, bits, len);
  EXPECT_EQ(len, 3);
  unsigned char rst2[] = {0b01000000};
  EXPECT_EQ(memcmp(bits, &rst2, len / 8 + 1), 0);
  memset(bits, 0, len / 8 + 1);

  golomb::get_uev_encode(6, bits, len);
  EXPECT_EQ(len, 5);
  unsigned char rst3[] = {0b00111000};
  EXPECT_EQ(memcmp(bits, &rst3, len / 8 + 1), 0);
  memset(bits, 0, len / 8 + 1);


  golomb::get_uev_encode(7, bits, len);
  EXPECT_EQ(len, 7);
  unsigned char rst4[] = {0b00010000};
  EXPECT_EQ(memcmp(bits, &rst4, len / 8 + 1), 0);
  memset(bits, 0, len / 8 + 1);

  golomb::get_uev_encode(15, bits, len);
  EXPECT_EQ(len, 9);
  unsigned char rst5[] = {0b00001000, 0b00000000};
  EXPECT_EQ(memcmp(bits, &rst5, len / 8 + 1), 0);
  memset(bits, 0, len / 8 + 1);

}

TEST(Golomb, Golomb_uev_decode_test) {
  unsigned char buf1[] = {0b10000000};
  auto rst = golomb::get_uev_decode(buf1, 0, 0);
  EXPECT_EQ(rst, 0);

  unsigned char buf2[] = {0b01000000};
  rst = golomb::get_uev_decode(buf2, 0, 0);
  EXPECT_EQ(rst, 1);

  unsigned char buf3[] = {0b00110000};
  rst = golomb::get_uev_decode(buf3, 0, 0);
  EXPECT_EQ(rst, 5);


  unsigned char buf4[] = {0b00010010};
  rst = golomb::get_uev_decode(buf4, 0, 0);
  EXPECT_EQ(rst, 8);

  unsigned char buf5[] = {0b00001000, 0b00000000};
  rst = golomb::get_uev_decode(buf5, 0, 0);
  EXPECT_EQ(rst, 15);
}

TEST(Golomb, Golomb_sev_encode_test) {
  auto * bits = new unsigned char;
  long len = -1;

  golomb::get_sev_encode(0, bits, len);
  EXPECT_EQ(len, 1);
  unsigned char rst1[] = {0x80};
  EXPECT_EQ(memcmp(bits, &rst1, len / 8 + 1), 0);
  memset(bits, 0, len / 8 + 1);

  golomb::get_sev_encode(4, bits, len);
  EXPECT_EQ(len, 7);
  unsigned char rst2[] = {0b00010000};
  EXPECT_EQ(memcmp(bits, &rst2, len / 8 + 1), 0);
  memset(bits, 0, len / 8 + 1);

  golomb::get_sev_encode(-15, bits, len);
  EXPECT_EQ(len, 9);
  unsigned char rst3[] = {0b00001111, 0b10000000};
  EXPECT_EQ(memcmp(bits, &rst3, len / 8 + 1), 0);
  memset(bits, 0, len / 8 + 1);

}

TEST(Golomb, Golomb_sev_decode_test) {
  unsigned char buf1[] = {0b10000000};
  auto rst = golomb::get_sev_decode(buf1, 0, 0);
  EXPECT_EQ(rst, 0);

  unsigned char buf2[] = {0b00010000};
  rst = golomb::get_sev_decode(buf2, 0, 0);
  EXPECT_EQ(rst, 4);


  unsigned char buf3[] = {0b00001111, 0b10000000};
  rst = golomb::get_sev_decode(buf3, 0, 0);
  EXPECT_EQ(rst, -15);
}