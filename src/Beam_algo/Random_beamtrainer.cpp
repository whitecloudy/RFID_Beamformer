#include "Beam_algo/Random_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define BaseNum (32)

Random_beamtrainer::Random_beamtrainer(int ant_num) : Beamtrainer(ant_num){
  avgCorrColumn.set_size(ant_num);
}

void Random_beamtrainer::printClassName(void){
  std::cout<<"Random Beamtrainer Selected!"<< std::endl;
}

const std::vector<int> Random_beamtrainer::startTraining(void){
  //reset all the values
  training_count = 0;
  randomWeightMatrix.reset();
  isTraining = true;

  curPhaseVector = getRandomWeight();

  return curPhaseVector;
}

/*
 *  Handle the tag's respond
 */
const std::vector<int> Random_beamtrainer::getRespond(struct average_corr_data recvData){
  curPhaseVector = getRandomWeight();

  return curPhaseVector;
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> Random_beamtrainer::cannotGetRespond(void){
  curPhaseVector = getRandomWeight();

  return curPhaseVector;
}
