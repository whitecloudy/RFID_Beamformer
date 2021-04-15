#include "Beam_algo/Directional_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>


Directional_beamtrainer::Directional_beamtrainer(int ant_num, std::vector<int> ant_array) : Beamtrainer(ant_num){
  this->ant_array = ant_array;
  this->cur_angle_x = 0;
  this->cur_angle_y = 0;
}

void Directional_beamtrainer::printClassName(void){
  std::cout << "Directional_beamtrainer Selected!!"<<std::endl;
}

void Directional_beamtrainer::reset_current_angles(void)
{
  cur_angle_x = -__BEAM_X_ANGLE_RANGE;
  cur_angle_y = -__BEAM_Y_ANGLE_RANGE;
}

const std::vector<int> Directional_beamtrainer::startTraining(void){
  //reset all the values
  reset_Directional_beamtrainer();
 
  isTraining = true;

  trainingPhaseVector = getDirectional(cur_angle_x, cur_angle_y);
  curPhaseVector = trainingPhaseVector;
  return curPhaseVector;
}


void Directional_beamtrainer::reset_Directional_beamtrainer(void)
{
  reset_Beamtrainer();
  reset_current_angles();
}


/*
 *  Handle the tag's respond
 */
const std::vector<int> Directional_beamtrainer::getRespond(struct average_corr_data recvData, std::vector<int> usedVector){
  trainingPhaseVector = getNextBeam();
  curPhaseVector = trainingPhaseVector;

  return curPhaseVector;
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> Directional_beamtrainer::cannotGetRespond(std::vector<int> usedVector){
  trainingPhaseVector = getNextBeam();
  curPhaseVector = trainingPhaseVector;

  return curPhaseVector;
}

std::vector<int> Directional_beamtrainer::getNextBeam(){ 
  cur_angle_x += __BEAM_ANGLE_STEP;
  if(cur_angle_x > __BEAM_X_ANGLE_RANGE){
    cur_angle_x = -__BEAM_X_ANGLE_RANGE;

    cur_angle_y += __BEAM_ANGLE_STEP;
    if(cur_angle_y > 0)
    {
      cur_angle_y = -__BEAM_Y_ANGLE_RANGE;
    }
  }

  return getDirectional(cur_angle_x, cur_angle_y);
}


std::vector<int> Directional_beamtrainer::getCurBeam(){ 
  return getDirectional(cur_angle_x, cur_angle_y);
}


/*
 * Make certain directional beam weight
 * 
 * This is only valid when the antennas are aligned in 1/2 lambda
 */
std::vector<int> Directional_beamtrainer::getDirectional(int angle_x, int angle_y){ 
  
  std::vector<int> weightVector(ant_num); 

  std::complex<double> xStepAngle = beam_util::phase2NormalComplex(180.0 * std::sin(Deg2Rad(angle_x))); 
  std::complex<double> yStepAngle = beam_util::phase2NormalComplex(2 * 180.0 * std::sin(Deg2Rad(angle_y)));   //be power this because the distance between antenna set in Y axis is wider in double

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

int Directional_beamtrainer::getBeamNum(void)
{
  int x_num = (cur_angle_x + __BEAM_X_ANGLE_RANGE)/__BEAM_ANGLE_STEP;
  int y_num = (cur_angle_y + __BEAM_Y_ANGLE_RANGE)/__BEAM_ANGLE_STEP;

  int x_range = (2 * __BEAM_X_ANGLE_RANGE)/__BEAM_ANGLE_STEP + 1;

  return (x_range*y_num) + x_num;
}

const std::vector<int> Directional_beamtrainer::beamNum2phaseVec(int beam_num)
{
  int x_range = (2 * __BEAM_X_ANGLE_RANGE)/__BEAM_ANGLE_STEP + 1;

  int x_num = beam_num%x_range;
  int y_num = beam_num/x_range;

  int angle_x = x_num*__BEAM_ANGLE_STEP - __BEAM_X_ANGLE_RANGE;
  int angle_y = y_num*__BEAM_ANGLE_STEP - __BEAM_Y_ANGLE_RANGE;

  return getDirectional(angle_x, angle_y);
}



std::vector<int> Directional_beamtrainer::getDirectional(int angle){ 

  return getDirectional(angle, 0); 
}

