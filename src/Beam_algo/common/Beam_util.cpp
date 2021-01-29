#include "Beam_util.hpp"

namespace beam_util
{
  /*
   * get phase from complex number
   * 
   * complexData : complex number that you want to get phase
   *
   * <rt> Phase vlaue in degree which is correspond to complex data
   */
  double complex2Phase(std::complex<double> complexData){
    double phase_before_process = (std::arg(complexData)/PI) * 180;
    if(phase_before_process < 0)
      phase_before_process+=360;
    return phase_before_process;
  }

  /*
   * get normalized complex number corresponding to phase(in degree)
   *
   * phaseData : number of phase in degree
   *
   * <rt> Normalized complex value which is correspond to phase
   */
  std::complex<double> phase2NormalComplex(double phaseData){
    return std::polar((double)1.0, (double)(phaseData*PI)/180);
  }
}
