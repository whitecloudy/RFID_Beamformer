#ifndef _BEAM_UTIL_
#define _BEAM_UTIL_

#include <complex>

#define PI  (3.14159265358979323846)
#define Deg2Rad(_num) (double)(_num * (PI / 180))
#define Rad2Deg(_num) (double)(_num / PI * 180.0)

namespace beam_util
{
  double complex2Phase(std::complex<double> complexData);
  std::complex<double> phase2NormalComplex(double phaseData);
}

#endif
