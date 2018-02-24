//
// Created by aLIEzTed on 2/24/18.
//

#ifndef SIMPLEH264_NALU_PARSER_H
#define SIMPLEH264_NALU_PARSER_H
#include <nalu.h>
#include <fstream>
#define NAL_HEADER_SHORT 0x000001
#define NAL_HEADER_LONG 0x00000001

int read_one_sodb(unsigned char* nalu, unsigned long& nalu_size);
int read_one_nalu(std::ifstream& file, unsigned long start, unsigned char*& nalu, unsigned long& nalu_size);
/**
 * return the nalu_header of nalu
 * @param nalu an unsigned char array holding data bits of nalu
 * @param nalu_size the length of unsigned char array counted in bytes.
 * @return the nalu_header, see nalu_header
 */
nalu_header parse_header(unsigned char* nalu, unsigned long nalu_size);


#endif //SIMPLEH264_NALU_PARSER_H
