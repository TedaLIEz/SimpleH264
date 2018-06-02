//
// Created by aLIEzTed on 6/2/18.
//

#ifndef SIMPLEH264_MATH_H
#define SIMPLEH264_MATH_H
namespace math {
inline unsigned CeilLog2(unsigned uiVal) {
  unsigned uiTmp = uiVal-1;
  unsigned uiRet = 0;

  while( uiTmp != 0 )
  {
    uiTmp >>= 1;
    uiRet++;
  }
  return uiRet;
}
}
#endif //SIMPLEH264_MATH_H
