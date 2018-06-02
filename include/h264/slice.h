//
// Created by aLIEzTed on 5/30/18.
//

#ifndef SIMPLEH264_SLICE_H
#define SIMPLEH264_SLICE_H
#include "sl_header.h"
#define SLICE_TYPE_P 0
#define SLICE_TYPE_B 1
#define SLICE_TYPE_I 2
#define SLICE_TYPE_SP 3
#define SLICE_TYPE_SI 4
class Slice {
  Slice_header hdr;
  int mb_skip_run;
};
#endif //SIMPLEH264_SLICE_H
