#include "Beam_algo/Directional_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define __BEAM_ANGLE_RANGE  (90)
#define __BEAM_ANGLE_STEP   (15)

Directional_beamtrainer::Directional_beamtrainer(int ant_num) : Beamtrainer(ant_num){
}

void Directional_beamtrainer::printClassName(void){
  std::cout << "Directional_beamtrainer Selected!!"<<std::endl;
}

const std::vector<int> Directional_beamtrainer::startTraining(void){
  //reset all the values
  current_angle = -__BEAM_ANGLE_RANGE;
  isTraining = true;

  curPhaseVector = getDirectional(current_angle);

  return curPhaseVector;
}

/*
 *  Handle the tag's respond
 */
const std::vector<int> Directional_beamtrainer::getRespond(struct average_corr_data recvData){
  return cannotGetRespond();
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> Directional_beamtrainer::cannotGetRespond(void){
  current_angle += __BEAM_ANGLE_STEP;
  if(current_angle > __BEAM_ANGLE_RANGE){
    current_angle = -__BEAM_ANGLE_RANGE;
  }

  curPhaseVector = getDirectional(current_angle);

  return curPhaseVector;
}


/*
 * Make certain directional beam weight
 * 
 * This is only valid when the antennas are aligned in 1/2 lambda
 */
std::vector<int> Directional_beamtrainer::getDirectional(double angle){ 
  std::vector<int> weightVector(ant_num); 
  std::complex<double> complexAngle(1,0); 
  std::complex<double> stepAngle = phase2NormalComplex(180.0 * std::sin(Deg2Rad(angle))); 

  for(int i = 0; i< ant_num; i++){ 
    weightVector[i] = complex2Phase(complexAngle); 
    complexAngle *= stepAngle; 
  } 

  return weightVector; 
}

