//
// Created by aLIEzTed on 2/24/18.
//

#ifndef SIMPLEH264_DEBUG_H
#define SIMPLEH264_DEBUG_H
#include <iostream>
#include <iomanip>

#ifdef DEBUG
#   define ASSERT(Expr, Msg)  ({ \
  if (!(Expr)) { \
    std::cerr << "Assert failed:\t" << (Msg) << "\n"  \
    << "Expected:\t" << #Expr << "\n" \
    << "Source:\t\t" << __FILE__ << ", line " << __LINE__ << "\n"; \
    abort(); \
  } \
})

#else
#   define M_Assert(Expr, Msg) ;
#endif


#ifdef DEBUG
#define PRINT_HEX(val) \
  std::ostream hexcout(std::cout.rdbuf()); \
  hexcout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (val) << std::endl;
#endif



#endif //SIMPLEH264_DEBUG_H
