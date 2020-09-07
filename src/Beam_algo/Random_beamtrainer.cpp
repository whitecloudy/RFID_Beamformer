#include "Beam_algo/Random_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define BaseNum (32)
#define PI  (3.14159265358979323846)

Random_beamtrainer::Random_beamtrainer(int ant_num) : Beamtrainer(ant_num){
  avgCorrColumn.set_size(ant_num);
}

const std::vector<int> Random_beamtrainer::startTraining(void){
  //reset all the values
  training_count = 0;
  randomWeightMatrix.reset();
  isTraining = true;

  return getRandomWeight();
}

/*
 *  Handle the tag's respond
 */
const std::vector<int> Random_beamtrainer::getRespond(struct average_corr_data recvData){
  return getRandomWeight();
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> Random_beamtrainer::cannotGetRespond(void){
  return getRandomWeight();
}


