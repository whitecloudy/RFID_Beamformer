#include "Beam_algo/Kalmaned_with_random_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define BaseNum (32)

Kalmaned_with_random_beamtrainer::Kalmaned_with_random_beamtrainer(int ant_num) 
  : Beamtrainer(ant_num), ca_cal(ant_num)
{
}

void Kalmaned_with_random_beamtrainer::printClassName(void){
  std::cout<<"Kalmaned_with_random Beamtrainer Selected!"<< std::endl;
}


const std::vector<int> Kalmaned_with_random_beamtrainer::startTraining(void){
  //reset all the values
  training_count = 0;
  ca_cal.clear();
  isTraining = true;

  optimal_used = false;
  trainingPhaseVector = getRandomWeight();
  curPhaseVector = trainingPhaseVector;
  return curPhaseVector;
}


/*
 *  Handle the tag's respond
 */
const std::vector<int> Kalmaned_with_random_beamtrainer::getRespond(struct average_corr_data recvData, std::vector<int> usedVector){
  std::complex<double> corrData(recvData.avg_i, recvData.avg_q);

  trainingPhaseVector = getRandomWeight();

  ca_cal.setNewTrainingVector(usedVector);
  training_count = ca_cal.setNewCorrData(corrData);

  if(ca_cal.is_processable())
  {
    optimalPhaseVector[0] = ca_cal.processOptimalVector();
    optimal_available = true;
  }

  optimal_used = false;
  curPhaseVector = trainingPhaseVector;
  return curPhaseVector;
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> Kalmaned_with_random_beamtrainer::cannotGetRespond(std::vector<int> usedVector){
  optimal_used = false;
  trainingPhaseVector = getRandomWeight();
  curPhaseVector = trainingPhaseVector;
  return curPhaseVector;
}
