#ifndef __STRUCT_GLOBAL__
#define __STRUCT_GLOBAL__

#define _SUCCESS 1
#define _GATE_FAIL 2
#define _PREAMBLE_FAIL 0

struct average_corr_data{
  char successFlag;
  char RN16[16];
  float avg_corr;
  float avg_i;
  float avg_q;
  unsigned int round;
  float cw_i;
  float cw_q;
  float stddev_i;
  float stddev_q;
};


#endif
