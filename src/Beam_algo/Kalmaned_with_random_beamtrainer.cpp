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

  curPhaseVector = getRandomWeight();
  ca_cal.setNewTrainingVector(curPhaseVector);

  return curPhaseVector;
}


/*
 *  Handle the tag's respond
 */
const std::vector<int> Kalmaned_with_random_beamtrainer::getRespond(struct average_corr_data recvData){
  std::complex<double> corrData(recvData.avg_i, recvData.avg_q);

  if(optimal_used)
  {
    curPhaseVector = getRandomWeight();
    ca_cal.setNewTrainingVector(curPhaseVector);
    optimal_used = false;
  }else
  {
    training_count = ca_cal.setNewCorrData(corrData);
    if(ca_cal.is_processable())
    {
      curPhaseVector = ca_cal.processOptimalVector();
      optimal_used = true;
    }else
    {
      curPhaseVector = getRandomWeight();
      ca_cal.setNewTrainingVector(curPhaseVector);
      optimal_used = false;
    }
  }

  return curPhaseVector;
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> Kalmaned_with_random_beamtrainer::cannotGetRespond(void){
  optimal_used = false;
  curPhaseVector = getRandomWeight();
  ca_cal.resetTrainingVector(curPhaseVector);

  return curPhaseVector;
}
