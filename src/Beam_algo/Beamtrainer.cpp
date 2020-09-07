#include "Beam_algo/Beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>


Beamtrainer::Beamtrainer(int ant_num){
  this->ant_num = ant_num;
  srand(time(NULL));

  optimalPhaseVector.resize(ant_num);
}

/*
 * get phase from complex number
 * 
 * complexData : complex number that you want to get phase
 *
 * <rt> Phase vlaue in degree which is correspond to complex data
 */
float Beamtrainer::complex2Phase(std::complex<float> complexData){
  float phase_before_process = (std::arg(complexData)/PI) * 180;
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
std::complex<float> Beamtrainer::phase2NormalComplex(float phaseData){
  return std::polar((float)1.0, (float)(phaseData*PI)/180);
}




/*
 * Set New RandomWeight on the last matrix row
 */
const std::vector<int> Beamtrainer::getRandomWeight(void){
  //set vector for phase shift
  std::vector<int> weightVector(ant_num);
  for(int i = 0; i < ant_num; i++){
    int phase = (rand()%1440)/4;
    weightVector[i] = phase;
  }

  return weightVector;
}

/*
 * Tell that the optimal Phase Vector is exist
 */
const bool Beamtrainer::isOptimalCalculated(void){
  return !optimalPhaseVector.empty();
}


const std::vector<int> Beamtrainer::getOptimalPhaseVector(void){
  return optimalPhaseVector;
}
