#ifndef __GLOBAL__
#define __GLOBAL__

#define ANT_num   (16)
#define MIN_POWER (-22.0)
#define MAX_POWER (-2.0)
#define DEFAULT_POWER (-3)

#define PI (3.1415926535897)

#define Deg2Rad(_num) (float)(_num * (PI / 180))
#define Rad2Deg(_num) (float)(_num / PI * 180.0)

#define dB2Amp(_X) (pow(10.0f, (_X)/20.0f))
#define Amp2dB(_X) (20.0f*log10(_X))


#define MIN_POWER_amp (dB2Amp(MIN_POWER))
#define MAX_POWER_amp (dB2Amp(MAX_POWER))


#endif
