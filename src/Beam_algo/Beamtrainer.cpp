#include "Beam_algo/Beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>



std::vector<int> Beamtrainer::curPhaseVector;

Beamtrainer::Beamtrainer(int ant_num){
  this->ant_num = ant_num;
  srand(time(NULL));

  optimalPhaseVector.resize(ant_num);
  curPhaseVector.resize(ant_num);
}


/*
 *
 * Generate Random Weight
 *
 * <rt> Generated random weight vector
 *
 */

std::vector<int> Beamtrainer::genRandomWeight(void){
  //set vector for phase shift
  std::vector<int> weightVector(ant_num);
  for(int i = 0; i < ant_num; i++){
    int phase = (rand()%1440)/4;
    weightVector[i] = phase;
  }

  return weightVector;
}


void Beamtrainer::reset_Beamtrainer(void)
{
  optimal_used = false;
  optimal_available = false;

  optimalPhaseVector.clear();
  trainingPhaseVector.clear();
  curPhaseVector.clear();
}


/*
 * Set New RandomWeight on the last matrix row
 */
const std::vector<int> Beamtrainer::getRandomWeight(void){
  optimal_used = false;
  curPhaseVector = genRandomWeight();
  return curPhaseVector;
}

/*
 * Tell that the optimal Phase Vector is exist
 */
const bool Beamtrainer::isOptimalCalculated(void){
  return optimal_available;
}

/*
 * Tell that the optimal Phase Vector is Used
 */
const bool Beamtrainer::isOptimalUsed(void){
  return optimal_used;
}


const std::vector<int> Beamtrainer::getOptimalPhaseVector(void){
  optimal_used = true;
  curPhaseVector = optimalPhaseVector;
  return optimalPhaseVector;
}


const std::vector<int> Beamtrainer::getTrainingPhaseVector(void){
  optimal_used = false;
  curPhaseVector = trainingPhaseVector;
  return trainingPhaseVector;
}

std::vector<int> Beamtrainer::beamPhaseShift(std::complex<double> angle, std::vector<int> phaseVector)
{
  double angles = Rad2Deg(std::arg(angle));

  return beamPhaseShift(angles, phaseVector);
}

std::vector<int> Beamtrainer::beamPhaseShift(double angle, std::vector<int> phaseVector)
{
  std::cout << "\n\n"<<phaseVector.size()<<std::endl;
  for(int i = 0; i < phaseVector.size(); i++)
  {
    phaseVector[i] += angle;
    while(phaseVector[i] < 0)
    {
      phaseVector[i] += 360;
    }

    while(phaseVector[i] >= 360)
    {
      phaseVector[i] -= 360;
    }
  }

  return phaseVector;
}
