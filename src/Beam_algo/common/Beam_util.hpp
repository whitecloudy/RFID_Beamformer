#ifndef _BEAM_UTIL_
#define _BEAM_UTIL_

#include <complex>
#include <global/global.hpp>


namespace beam_util
{
  double complex2Phase(std::complex<double> complexData);
  std::complex<double> phase2NormalComplex(double phaseData);
  double phaseTrimming(double phaseData);
}

#endif
