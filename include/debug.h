//
// Created by aLIEzTed on 2/24/18.
//

#ifndef SIMPLEH264_DEBUG_H
#define SIMPLEH264_DEBUG_H
#include <iostream>
#include <iomanip>
#ifdef DEBUG
#define PRINT_HEX(val) \
  std::ostream hexcout(std::cout.rdbuf()); \
  hexcout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (val) << std::endl;
#endif
#endif //SIMPLEH264_DEBUG_H
