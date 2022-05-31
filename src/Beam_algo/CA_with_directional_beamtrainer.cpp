#include "Beam_algo/CA_with_directional_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define __SCRAMBLE_VAR      (30)

CA_with_directional_beamtrainer::CA_with_directional_beamtrainer(int ant_num, std::vector<int> ant_array, int round_max) : Directional_beamtrainer(ant_num, ant_array), ca_cal(ant_num, 0), curCenterPhaseVector(ant_num){
  this->round_max = round_max;
  this->best_beam_max = 3;
}

void CA_with_directional_beamtrainer::printClassName(void){
  std::cout << "CA_with_directional_beamtrainer Selected!!"<<std::endl;
}

const std::vector<int> CA_with_directional_beamtrainer::startTraining(void){
  //reset all the values
  reset_CA_with_directional_beamtrainer();

  isTraining = true;
  
  trainingPhaseVector = getDirectional(cur_angle_x, cur_angle_y);
  curPhaseVector = trainingPhaseVector;

  return curPhaseVector;
}


void CA_with_directional_beamtrainer::reset_CA_with_directional_beamtrainer(void)
{
  reset_Directional_beamtrainer();

  round_count = 0;
  best_beam_count = 0;

  ca_cal.clear();

  rankBeamL.clear();
  bestBeamNum.clear();

  curCenterPhaseVector.clear();

  beamSearchFlag = true;
}



/*
 *  Handle the tag's respond
 */
const std::vector<int> CA_with_directional_beamtrainer::getRespond(struct average_corr_data recvData, std::vector<int> usedVector){

  if(beamSearchFlag)
  {
    for (auto beamIter = rankBeamL.begin(); true; beamIter++)
    {
      if(((*beamIter).amp < recvData.avg_corr)||(beamIter == rankBeamL.end()))
      {
        struct beamStruct tmpData;

        tmpData.beamNum = getBeamNum();
        tmpData.amp = recvData.avg_corr;
        rankBeamL.insert(beamIter, tmpData);

        break;
      }

    }

    if(!optimal_used)
    {
      ca_cal.setNewTrainingVector(usedVector);
      ca_cal.setNewCorrData(std::complex<double>(recvData.avg_i, recvData.avg_q));
    }

    trainingPhaseVector = getNextBeam();

    int beamNum = getBeamNum();

    if((beamNum == 0)&&(rankBeamL.size() != 0))
    {
      round_count = round_max;
      best_beam_count = 0;

      for(int i = 0; i < rankBeamL.size(); i++)
      {
        bestBeamNum.push_back(rankBeamL.front().beamNum);
        rankBeamL.pop_front();
      }

      rankBeamL.clear();

      curCenterPhaseVector = beamNum2phaseVec(bestBeamNum[best_beam_count % bestBeamNum.size()]);
      trainingPhaseVector = randomScramble(curCenterPhaseVector, __SCRAMBLE_VAR);

      beamSearchFlag = false;
    }
  }else
  {
    if(!optimal_used)
    {
      ca_cal.setNewTrainingVector(usedVector);
      ca_cal.setNewCorrData(std::complex<double>(recvData.avg_i, recvData.avg_q));
    }

    if(ca_cal.is_processable() && !optimal_used)
    {
      optimalPhaseVector[0] = ca_cal.processOptimalVector();
      optimal_available = true;
    }

    if(round_count > 0)                  //scramble with random var
    {
      //TODO : someday should i have to get std value with parameter??
      trainingPhaseVector = randomScramble(curCenterPhaseVector, __SCRAMBLE_VAR);
      round_count--;
    }
    else    //shift center beam
    {
      round_count = round_max;
      best_beam_count++; 

      curCenterPhaseVector = beamNum2phaseVec(bestBeamNum[best_beam_count % bestBeamNum.size()]);
      trainingPhaseVector = randomScramble(curCenterPhaseVector, __SCRAMBLE_VAR);
    } 

  }

  optimal_used = false;
  curPhaseVector = trainingPhaseVector;
  return curPhaseVector;
}


/*
 * Handle when the tag does not respond
 */
const std::vector<int> CA_with_directional_beamtrainer::cannotGetRespond(std::vector<int> usedVector){
  if(beamSearchFlag)
  {
    trainingPhaseVector = getNextBeam();

    int beamNum = getBeamNum();

    if((beamNum == 0)&&(rankBeamL.size() != 0))
    {
      round_count = round_max;
      best_beam_count = 0;

      for(int i = 0; i < rankBeamL.size(); i++)
      {
        bestBeamNum.push_back(rankBeamL.front().beamNum);
        rankBeamL.pop_front();
      }

      rankBeamL.clear();

      curCenterPhaseVector = beamNum2phaseVec(bestBeamNum[best_beam_count % bestBeamNum.size()]);
      trainingPhaseVector = randomScramble(curCenterPhaseVector, __SCRAMBLE_VAR);

      beamSearchFlag = false;
    }
  }else
  {
    if(round_count > 0)                  //scramble with random var
    {
      //TODO : someday should i have to get std value with parameter??
      trainingPhaseVector = randomScramble(curCenterPhaseVector, __SCRAMBLE_VAR);
      round_count--;
    }
    else    //shift center beam
    {
      round_count = round_max;
      best_beam_count++; 

      curCenterPhaseVector = beamNum2phaseVec(bestBeamNum[best_beam_count % bestBeamNum.size()]);
      trainingPhaseVector = randomScramble(curCenterPhaseVector, __SCRAMBLE_VAR);
    } 
  }


  optimal_used = false;
  curPhaseVector = trainingPhaseVector;
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
