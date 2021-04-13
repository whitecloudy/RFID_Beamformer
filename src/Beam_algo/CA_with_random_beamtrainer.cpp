#include "CA_with_random_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define BaseNum (32)

CA_with_random_beamtrainer::CA_with_random_beamtrainer(int ant_num) : Beamtrainer(ant_num), ca_cal(ant_num, 0){
}

void CA_with_random_beamtrainer::printClassName(void){
  std::cout<<"CA_with_random Beamtrainer Selected!"<< std::endl;
}



const std::vector<int> CA_with_random_beamtrainer::startTraining(void){
  //reset all the values
  reset_CA_with_random_beamtrainer();

  isTraining = true;

  trainingPhaseVector = getRandomWeight();

  std::cout<< "Ready"<<std::endl;

  curPhaseVector = trainingPhaseVector;
  return curPhaseVector;
}

void CA_with_random_beamtrainer::reset_CA_with_random_beamtrainer(void)
{
  reset_Beamtrainer();

  training_count = 0;
  ca_cal.clear();
}


/*
 *  Handle the tag's respond
 */
const std::vector<int> CA_with_random_beamtrainer::getRespond(struct average_corr_data recvData, std::vector<int> usedVector){
  std::complex<double> corrData(recvData.avg_i, recvData.avg_q);

  optimal_used = false;

  trainingPhaseVector = getRandomWeight();

  ca_cal.setNewTrainingVector(usedVector);
  training_count = ca_cal.setNewCorrData(corrData);

  if(ca_cal.is_processable())
  {
    optimalPhaseVector = ca_cal.processOptimalVector();
    optimal_available = true;
  }

  curPhaseVector = trainingPhaseVector;
  return curPhaseVector;
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> CA_with_random_beamtrainer::cannotGetRespond(std::vector<int> usedVector){
  optimal_used = false;
  trainingPhaseVector = getRandomWeight();
  curPhaseVector = trainingPhaseVector;
  return curPhaseVector;
}
