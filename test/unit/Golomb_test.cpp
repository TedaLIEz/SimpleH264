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
  EXPECT_EQ(bits[0], 0x80);
  memset(bits, 0, len / 8 + 1);

  golomb::get_uev_encode(1, bits, len);
  EXPECT_EQ(len, 3);
  EXPECT_EQ(bits[0], 0b01000000);
  memset(bits, 0, len / 8 + 1);

  golomb::get_uev_encode(6, bits, len);
  EXPECT_EQ(len, 5);
  EXPECT_EQ(bits[0], 0b00111000);
  memset(bits, 0, len / 8 + 1);


  golomb::get_uev_encode(7, bits, len);
  EXPECT_EQ(len, 7);
  EXPECT_EQ(bits[0], 0b00010000);
  memset(bits, 0, len / 8 + 1);

  golomb::get_uev_encode(15, bits, len);
  EXPECT_EQ(len, 9);
  EXPECT_EQ(bits[0], 0b00001000);
  EXPECT_EQ(bits[1], 0x00);
  memset(bits, 0, len / 8 + 1);

}

TEST(Golomb, Golomb_uev_decode_test) {
  unsigned char buf1[] = {0b10000000};
  long len = -1;
  auto rst = golomb::get_uev_decode(buf1, 0, 0, len);
  EXPECT_EQ(rst, 0);
  EXPECT_EQ(len, 1);

  unsigned char buf2[] = {0b01000000};
  rst = golomb::get_uev_decode(buf2, 0, 0, len);
  EXPECT_EQ(rst, 1);
  EXPECT_EQ(len, 3);

  unsigned char buf3[] = {0b00110000};
  rst = golomb::get_uev_decode(buf3, 0, 0, len);
  EXPECT_EQ(rst, 5);
  EXPECT_EQ(len, 5);


  unsigned char buf4[] = {0b00010010};
  rst = golomb::get_uev_decode(buf4, 0, 0, len);
  EXPECT_EQ(rst, 8);
  EXPECT_EQ(len, 7);

  unsigned char buf5[] = {0b00001000, 0b00000000};
  rst = golomb::get_uev_decode(buf5, 0, 0, len);
  EXPECT_EQ(rst, 15);
  EXPECT_EQ(len, 9);

  unsigned char buf6[] = {0x84, 0x05};
  rst = golomb::get_uev_decode(buf6, 0, 1, len);
  EXPECT_EQ(rst, 15);
  EXPECT_EQ(len, 9);
}

TEST(Golomb, Golomb_sev_encode_test) {
  auto * bits = new unsigned char;
  long len = -1;

  golomb::get_sev_encode(0, bits, len);
  EXPECT_EQ(len, 1);
  EXPECT_EQ(bits[0], 0x80);
  memset(bits, 0, len / 8 + 1);

  golomb::get_sev_encode(4, bits, len);
  EXPECT_EQ(len, 7);
  EXPECT_EQ(bits[0], 0b00010000);
  memset(bits, 0, len / 8 + 1);

  golomb::get_sev_encode(-15, bits, len);
  EXPECT_EQ(len, 9);
  EXPECT_EQ(bits[0], 0b00001111);
  EXPECT_EQ(bits[1], 0b10000000);
  memset(bits, 0, len / 8 + 1);

}

TEST(Golomb, Golomb_sev_decode_test) {
  unsigned char buf1[] = {0b10000000};
  long len = -1;
  auto rst = golomb::get_sev_decode(buf1, 0, 0, len);
  EXPECT_EQ(rst, 0);
  EXPECT_EQ(len, 1);

  unsigned char buf2[] = {0b00010000};
  rst = golomb::get_sev_decode(buf2, 0, 0, len);
  EXPECT_EQ(rst, 4);
  EXPECT_EQ(len, 7);


  unsigned char buf3[] = {0b00001111, 0b10000000};
  rst = golomb::get_sev_decode(buf3, 0, 0, len);
  EXPECT_EQ(rst, -15);
  EXPECT_EQ(len, 9);

  unsigned char buf4[] = {0x84, 0x05};
  rst = golomb::get_sev_decode(buf4, 0, 1, len);
  EXPECT_EQ(rst, 8);
  EXPECT_EQ(len, 9);

  unsigned char buf5[] = {0xb8, 0xac};
  rst = golomb::get_sev_decode(buf5, 0, 5, len);
  EXPECT_EQ(rst, 5);
  EXPECT_EQ(len, 7);
}