#include "Beam_algo/Test_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define BaseNum (32)
#define PI  (3.14159265358979323846)

Test_beamtrainer::Test_beamtrainer(int ant_num) 
  : Beamtrainer(ant_num), ca_cal(ant_num), kalman_ca_cal(ant_num)
{
}

void Test_beamtrainer::printClassName(void){
  std::cout<<"Test Beamtrainer Selected!"<< std::endl;
}


const std::vector<int> Test_beamtrainer::startTraining(void){
  //reset all the values
  training_count = 0;
  ca_cal.clear();
  isTraining = true;

  curPhaseVector = getRandomWeight();
  ca_cal.setNewTrainingVector(curPhaseVector);
  kalman_ca_cal.setNewTrainingVector(curPhaseVector);


  return curPhaseVector;
}


/*
 *  Handle the tag's respond
 */
const std::vector<int> Test_beamtrainer::getRespond(struct average_corr_data recvData){
  std::complex<double> corrData(recvData.avg_i, recvData.avg_q);

  if(optimal_used)
  {
    if(kalman_ca_optimal) 
    {
      if(ca_cal.is_processable())  //CA turn
      {
        curPhaseVector = ca_cal.processOptimalVector();
        ca_optimal = true;
        kalman_optimal = false;
        optimal_used = true;
      }else
      {
        curPhaseVector = getRandomWeight();
        ca_cal.setNewTrainingVector(curPhaseVector);
        kalman_ca_cal.setNewTrainingVector(curPhaseVector);
        optimal_used = false;
        ca_optimal = false;
        kalman_optimal = false;
      }
    }else if(ca_optimal) //Optimal phase over
    {
      curPhaseVector = getRandomWeight();
      ca_cal.setNewTrainingVector(curPhaseVector);
      kalman_ca_cal.setNewTrainingVector(curPhaseVector);
      optimal_used = false;
      ca_optimal = false;
      kalman_optimal = false;
    }
  }else
  {
    training_count = ca_cal.setNewCorrData(corrData);
    training_count = kalman_ca_cal.setNewCorrData(corrData);

    if(kalman_ca_cal.is_processable())    //kalman turn
    {
      curPhaseVector = kalman_ca_cal.processOptimalVector();
      ca_optimal = false;
      kalman_optimal = true;
      optimal_used = true;
    }else
    {
      curPhaseVector = getRandomWeight();
      ca_cal.setNewTrainingVector(curPhaseVector);
      kalman_ca_cal.setNewTrainingVector(curPhaseVector);
      optimal_used = false;
      ca_optimal = false;
      kalman_optimal = false;
    }
  }

  return curPhaseVector;
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> Test_beamtrainer::cannotGetRespond(void){

  if(optimal_used)
  {
    if(kalman_ca_optimal) //it was CA turn
    {
      if(ca_cal.is_processable())
      {
        curPhaseVector = ca_cal.processOptimalVector();
        ca_optimal = true;
        kalman_optimal = false;
        optimal_used = true;
      }else
      {
        curPhaseVector = getRandomWeight();
        ca_cal.setNewTrainingVector(curPhaseVector);
        kalman_ca_cal.setNewTrainingVector(curPhaseVector);
        optimal_used = false;
        ca_optimal = false;
        kalman_optimal = false;
      }
    }else if(kalman_optimal) //it was kalman turn
    {
      curPhaseVector = getRandomWeight();
      ca_cal.setNewTrainingVector(curPhaseVector);
      kalman_ca_cal.setNewTrainingVector(curPhaseVector);
      optimal_used = false;
      ca_optimal = false;
      kalman_optimal = false;
    }
  }else
  {
    optimal_used = false;
    ca_optimal = false;
    kalman_optimal = false;
    curPhaseVector = getRandomWeight();
    ca_cal.resetTrainingVector(curPhaseVector);
    kalman_ca_cal.resetTrainingVector(curPhaseVector);
  }

  return curPhaseVector;
}


const int Test_beamtrainer::which_optimal(void)
{
  if(!optimal_used)
    return 0;
  else if(ca_optimal)
    return 1;
  else if(kalman_optimal)
    return 2;
}
