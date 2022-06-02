#ifndef __STRUCT_GLOBAL__
#define __STRUCT_GLOBAL__

#include <vector>
#include <string>
#include <iostream>

#define _SUCCESS 1
#define _GATE_FAIL 2
#define _PREAMBLE_FAIL 0

struct average_corr_data{
  uint8_t successFlag;
  uint8_t data_flag;
  //char RN16[16];
  uint8_t data_bits[128];
  float avg_corr;
  float avg_i;
  float avg_q;
  uint32_t round;
  float cw_i;
  float cw_q;
  float stddev_i;
  float stddev_q;
};





#endif
