#include "Beam_algo/CA_with_directional_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define __BEAM_ANGLE_RANGE  (90)
#define __BEAM_ANGLE_STEP   (10)
#define __SCRAMBLE_VAR      (30)

CA_with_directional_beamtrainer::CA_with_directional_beamtrainer(int ant_num, int round_max) : Directional_beamtrainer(ant_num), ca_cal(ant_num){
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
  
  ca_cal.clear();

  curPhaseVector = getDirectional(current_angle);
  ca_cal.setNewTrainingVector(curPhaseVector);

  return curPhaseVector;
}


/*
 *  Handle the tag's respond
 */
const std::vector<int> CA_with_directional_beamtrainer::getRespond(struct average_corr_data recvData){

  if(ca_cal.is_processable() && !optimal_used)
  {
    ca_cal.setNewCorrData(std::complex<double>(recvData.avg_i, recvData.avg_q));
    curPhaseVector = ca_cal.processOptimalVector();
    optimal_used = true;
  }else
  {
    if(!optimal_used)
      ca_cal.setNewCorrData(std::complex<double>(recvData.avg_i, recvData.avg_q));

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
      curPhaseVector = randomScramble(curCenterPhaseVector, __SCRAMBLE_VAR);
      round_count--;
    }

    ca_cal.setNewTrainingVector(curPhaseVector);
    optimal_used = false;
  }

  return curPhaseVector;
}


/*
 * Handle when the tag does not respond
 */
const std::vector<int> CA_with_directional_beamtrainer::cannotGetRespond(void){
  optimal_used = false;
  if((round_count <= 0) || (curCenterPhaseVector == curPhaseVector))    //shift center beam
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
    curPhaseVector = randomScramble(curCenterPhaseVector, __SCRAMBLE_VAR);
    round_count--;
  }
  ca_cal.resetTrainingVector(curPhaseVector);

  return curPhaseVector;
}



/*
 * Scramble the phase vector with random phase
 *
 */
std::vector<int> CA_with_directional_beamtrainer::randomScramble(std::vector<int> center, double std){
  static std::default_random_engine random_gen;

  std::normal_distribution<double> normal_random(0, std);

  for(int idx = 0; idx < center.size(); idx++){
    center[idx] += normal_random(random_gen);
  }

  return center;
}
