#include "Beam_algo/CA_with_directional_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define __BEAM_ANGLE_RANGE  (90)
#define __BEAM_ANGLE_STEP   (10)

CA_with_directional_beamtrainer::CA_with_directional_beamtrainer(int ant_num, int round_max) : Directional_beamtrainer(ant_num){
  this->round_max = round_max;

  curCenterPhaseVector.resize(ant_num);
}

void CA_with_directional_beamtrainer::printClassName(void){
  std::cout << "CA_with_directional_beamtrainer Selected!!"<<std::endl;
}

const std::vector<int> CA_with_directional_beamtrainer::startTraining(void){
  //reset all the values
  round_count = 0;
  current_angle = -__BEAM_ANGLE_RANGE;
  isTraining = true;

  curPhaseVector = getDirectional(current_angle);

  return curPhaseVector;
}


/*
 *  Handle the tag's respond
 */
const std::vector<int> CA_with_directional_beamtrainer::getRespond(struct average_corr_data recvData){
  return cannotGetRespond();
}


/*
 * Handle when the tag does not respond
 */
const std::vector<int> CA_with_directional_beamtrainer::cannotGetRespond(void){
  if(round_count <= 0)    //shift center beam
  {
    current_angle += __BEAM_ANGLE_STEP;
    if(current_angle > __BEAM_ANGLE_RANGE){
      current_angle = -__BEAM_ANGLE_RANGE;
    }

    //calculate new phase vector
    curCenterPhaseVector = getDirectional(current_angle);
    curPhaseVector = curCenterPhaseVector;
    round_count = round_max;   //reset round counter
  }else                   //scramble with random var
  {
    //TODO : someday should i have to get std value with parameter??
    curPhaseVector = randomScramble(curCenterPhaseVector, 10);
    round_count--;
  }

  return curPhaseVector;
}



/*
 * Scramble the phase vector with random phase
 *
 */
std::vector<int> CA_with_directional_beamtrainer::randomScramble(std::vector<int> center, float std){
  static std::default_random_engine random_gen;
  
  std::normal_distribution<float> normal_random(0, std);

  for(int idx = 0; idx < center.size(); idx++){
    center[idx] += normal_random(random_gen);
  }

  return center;
}