#include "Beam_algo/Directional_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define __BEAM_ANGLE_RANGE  (90)
#define __BEAM_ANGLE_STEP   (15)

Directional_beamtrainer::Directional_beamtrainer(int ant_num) : Beamtrainer(ant_num){
  avgCorrColumn.set_size(ant_num);
}

const std::vector<int> Directional_beamtrainer::startTraining(void){
  //reset all the values
  current_angle = -__BEAM_ANGLE_RANGE;
  randomWeightMatrix.reset();
  isTraining = true;

  return getDirectional(current_angle);
}

/*
 *  Handle the tag's respond
 */
const std::vector<int> Directional_beamtrainer::getRespond(struct average_corr_data recvData){
  current_angle += __BEAM_ANGLE_STEP;
  if(current_angle > __BEAM_ANGLE_RANGE){
    current_angle = -__BEAM_ANGLE_RANGE;
  }

  return getDirectional(current_angle);
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> Directional_beamtrainer::cannotGetRespond(void){
  current_angle += __BEAM_ANGLE_STEP;
  if(current_angle > __BEAM_ANGLE_RANGE){
    current_angle = -__BEAM_ANGLE_RANGE;
  }

  return getDirectional(current_angle);
}


/*
 * Make certain directional beam weight
 * 
 * This is only valid when the antennas are aligned in 1/2 lambda
 */

std::vector<int> Directional_beamtrainer::getDirectional(float angle){ 
  std::vector<int> weightVector(ant_num); 
  std::complex<float> complexAngle(1,0); 
  std::complex<float> stepAngle = phase2NormalComplex(180.0 * std::sin(Deg2Rad(angle))); 

  for(int i = 0; i< ant_num; i++){ 
    weightVector[i] = complex2Phase(complexAngle); 
    complexAngle *= stepAngle; 
  } 

  return weightVector; 
}

