#include "CA_with_random_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define BaseNum (32)

CA_with_random_beamtrainer::CA_with_random_beamtrainer(int ant_num) : Beamtrainer(ant_num), ca_cal(ant_num, 3){
}

void CA_with_random_beamtrainer::printClassName(void){
  std::cout<<"CA_with_random Beamtrainer Selected!"<< std::endl;
}



const std::vector<int> CA_with_random_beamtrainer::startTraining(void){
  //reset all the values
  training_count = 0;
  ca_cal.clear();
  isTraining = true;

  curPhaseVector = getRandomWeight();

  return curPhaseVector;
}


/*
 *  Handle the tag's respond
 */
const std::vector<int> CA_with_random_beamtrainer::getRespond(struct average_corr_data recvData){
  std::complex<double> corrData(recvData.avg_i, recvData.avg_q);

  if(optimal_used)
  {
    curPhaseVector = getRandomWeight();
    optimal_used = false;
  }else
  {
    ca_cal.setNewTrainingVector(curPhaseVector);
    training_count = ca_cal.setNewCorrData(corrData);
    if(ca_cal.is_processable())
    {
      curPhaseVector = ca_cal.processOptimalVector();
      optimal_used = true;
    }else
    {
      curPhaseVector = getRandomWeight();
      optimal_used = false;
    }
  }

  return curPhaseVector;
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> CA_with_random_beamtrainer::cannotGetRespond(void){
  optimal_used = false;
  curPhaseVector = getRandomWeight();

  return curPhaseVector;
}
