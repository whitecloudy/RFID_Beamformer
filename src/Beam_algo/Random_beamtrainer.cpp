#include "Beam_algo/Random_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define BaseNum (32)

Random_beamtrainer::Random_beamtrainer(int ant_num) : Beamtrainer(ant_num){
}

void Random_beamtrainer::printClassName(void){
  std::cout<<"Random Beamtrainer Selected!"<< std::endl;
}

const std::vector<int> Random_beamtrainer::startTraining(void){
  //reset all the values
  isTraining = true;

  trainingPhaseVector = getRandomWeight();
  curPhaseVector = trainingPhaseVector;

  return curPhaseVector;
}

/*
 *  Handle the tag's respond
 */
const std::vector<int> Random_beamtrainer::getRespond(struct average_corr_data recvData, std::vector<int> usedVector){
  trainingPhaseVector = getRandomWeight();
  curPhaseVector = trainingPhaseVector;
  return curPhaseVector;
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> Random_beamtrainer::cannotGetRespond(std::vector<int> usedVector){
  trainingPhaseVector = getRandomWeight();
  curPhaseVector = trainingPhaseVector;
  return curPhaseVector;
}
