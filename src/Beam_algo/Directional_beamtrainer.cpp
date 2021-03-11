#include "Beam_algo/Directional_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define __BEAM_X_ANGLE_RANGE  (90)
#define __BEAM_Y_ANGLE_RANGE  (30)
#define __BEAM_ANGLE_STEP   (15)

Directional_beamtrainer::Directional_beamtrainer(int ant_num, std::vector<int> ant_array) : Beamtrainer(ant_num), current_angles(2){
  this->ant_array = ant_array;
}

void Directional_beamtrainer::printClassName(void){
  std::cout << "Directional_beamtrainer Selected!!"<<std::endl;
}

void Directional_beamtrainer::reset_current_angles(void)
{
  current_angles[0] = -__BEAM_X_ANGLE_RANGE;
  current_angles[1] = -__BEAM_Y_ANGLE_RANGE;
}

const std::vector<int> Directional_beamtrainer::startTraining(void){
  //reset all the values
  reset_current_angles();
 
  isTraining = true;

  curPhaseVector = getDirectional(current_angles);

  return curPhaseVector;
}

/*
 *  Handle the tag's respond
 */
const std::vector<int> Directional_beamtrainer::getRespond(struct average_corr_data recvData){
  curPhaseVector = getNextBeam();

  return curPhaseVector;
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> Directional_beamtrainer::cannotGetRespond(void){
  curPhaseVector = getNextBeam();

  return curPhaseVector;
}

std::vector<int> Directional_beamtrainer::getNextBeam(){ 
  current_angles[0] += __BEAM_ANGLE_STEP;
  if(current_angles[0] > __BEAM_X_ANGLE_RANGE){
    current_angles[1] += __BEAM_ANGLE_STEP;
    if(current_angles[1] > __BEAM_Y_ANGLE_RANGE)
    {
      current_angles[1] = -__BEAM_Y_ANGLE_RANGE;
    }
    current_angles[0] = -__BEAM_X_ANGLE_RANGE;
  }

  return getDirectional(current_angles);
}



/*
 * Make certain directional beam weight
 * 
 * This is only valid when the antennas are aligned in 1/2 lambda
 */
std::vector<int> Directional_beamtrainer::getDirectional(std::vector<int> angles){ 
  if(angles.size() != ant_array.size())
  {
    std::cerr << "wrong angle received"<<std::endl;
    exit(1);
  }

  std::vector<int> weightVector(ant_num); 

  std::complex<double> xStepAngle = beam_util::phase2NormalComplex(180.0 * std::sin(Deg2Rad(angles[0]))); 
  std::complex<double> yStepAngle = beam_util::phase2NormalComplex(180.0 * std::sin(Deg2Rad(angles[1]))); 

  //Setting X Y angles
  std::complex<double> xComplexAngle(1,0); 
  for(int i = 0; i < ant_array[0]; i++){
    std::complex<double> yComplexAngle(1,0); 
    for(int j = 0; j < ant_array[1]; j++)
    {
      weightVector[i + j*ant_array[0]] = beam_util::complex2Phase(xComplexAngle * yComplexAngle); 
      yComplexAngle *= yStepAngle;
    }
    xComplexAngle *= xStepAngle; 
  }

  return weightVector; 
}


std::vector<int> Directional_beamtrainer::getDirectional(int angle){ 

  return getDirectional({angle, 0}); 
}

