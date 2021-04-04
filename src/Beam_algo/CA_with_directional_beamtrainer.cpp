#include "Beam_algo/CA_with_directional_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define __SCRAMBLE_VAR      (30)

CA_with_directional_beamtrainer::CA_with_directional_beamtrainer(int ant_num, std::vector<int> ant_array, int round_max) : Directional_beamtrainer(ant_num, ant_array), ca_cal(ant_num, 3), curCenterPhaseVector(ant_num){
  this->round_max = round_max;
  this->best_beam_max = 3;
}

void CA_with_directional_beamtrainer::printClassName(void){
  std::cout << "CA_with_directional_beamtrainer Selected!!"<<std::endl;
}

const std::vector<int> CA_with_directional_beamtrainer::startTraining(void){
  //reset all the values
  round_count = 0;

  reset_current_angles();

  isTraining = true;
  
  ca_cal.clear();

  curPhaseVector = getDirectional(cur_angle_x, cur_angle_y);

  return curPhaseVector;
}


/*
 *  Handle the tag's respond
 */
const std::vector<int> CA_with_directional_beamtrainer::getRespond(struct average_corr_data recvData){

  if(beamSearchFlag)
  {

    std::list<int>::iterator numIter = rankBeamNum.begin();
    std::list<double>::iterator ampIter = rankBeamAmp.begin();

    for (int beamAmp : rankBeamAmp)
    {
      if(beamAmp < recvData.avg_corr)
        break;

      numIter++;
      ampIter++;
    }

    int beamNum = getBeamNum();

    rankBeamNum.insert(numIter, beamNum);
    rankBeamAmp.insert(ampIter, recvData.avg_corr);

    curPhaseVector = getNextBeam();

    beamNum = getBeamNum();

    if((beamNum == 0)&&(rankBeamNum.size() != 0))
    {

      round_count = round_max;
      best_beam_count = best_beam_max;

      curCenterPhaseVector = beamNum2phaseVec(rankBeamNum.front());
      rankBeamNum.pop_front();
      curPhaseVector = curCenterPhaseVector;

      beamSearchFlag = false;
    }
  }else
  {
    if(!optimal_used)
    {
      ca_cal.setNewTrainingVector(curPhaseVector);
      ca_cal.setNewCorrData(std::complex<double>(recvData.avg_i, recvData.avg_q));
    }

    if(ca_cal.is_processable() && !optimal_used)
    {
      curPhaseVector = ca_cal.processOptimalVector();
      optimal_used = true;
    }else
    {
      if(round_count > 0)                  //scramble with random var
      {
        //TODO : someday should i have to get std value with parameter??
        curPhaseVector = randomScramble(curCenterPhaseVector, __SCRAMBLE_VAR);
        round_count--;
      }
      else    //shift center beam
      {
        //calculate new phase vector
        round_count = round_max;
        best_beam_count--;

        if(best_beam_count > 0) //Use Next best beam
        {
          if(rankBeamNum.size() != 0)
          {
            curCenterPhaseVector = beamNum2phaseVec(rankBeamNum.front());
            rankBeamNum.pop_front();
            curPhaseVector = curCenterPhaseVector;
          }else{ //If we run out of center beam, just use previous one
            curPhaseVector = randomScramble(curCenterPhaseVector, __SCRAMBLE_VAR);
          }
                  }else   //back to search mode
        {
          curPhaseVector = getCurBeam();

          rankBeamNum.clear();
          rankBeamAmp.clear();
          beamSearchFlag = true;
        }
      }
      optimal_used = false;
    }
  }

  /*
  */

  return curPhaseVector;
}


/*
 * Handle when the tag does not respond
 */
const std::vector<int> CA_with_directional_beamtrainer::cannotGetRespond(void){
  if(beamSearchFlag)
  {
    curPhaseVector = getNextBeam();

    int beamNum = getBeamNum();

    if((beamNum == 0)&&(rankBeamNum.size() != 0))
    {

      round_count = round_max;
      best_beam_count = best_beam_max;

      curCenterPhaseVector = beamNum2phaseVec(rankBeamNum.front());
      rankBeamNum.pop_front();
      curPhaseVector = curCenterPhaseVector;

      beamSearchFlag = false;
    }
  }else
  {
    optimal_used = false;

    if(round_count > 0)                  //scramble with random var
    {
      //TODO : someday should i have to get std value with parameter??
      curPhaseVector = randomScramble(curCenterPhaseVector, __SCRAMBLE_VAR);
      round_count--;
    }
    else    //shift center beam
    {
      //calculate new phase vector
      round_count = round_max;
      best_beam_count--;

      if(best_beam_count > 0) //Use Next best beam
      {
        if(rankBeamNum.size() != 0)
        {
          curCenterPhaseVector = beamNum2phaseVec(rankBeamNum.front());
          rankBeamNum.pop_front();
          curPhaseVector = curCenterPhaseVector;
        }else{ //If we run out of center beam, just use previous one
          curPhaseVector = randomScramble(curCenterPhaseVector, __SCRAMBLE_VAR);
        }
      }else   //back to search mode
      {
        curPhaseVector = getCurBeam();

        rankBeamNum.clear();
        rankBeamAmp.clear();
        beamSearchFlag = true;
      }
    }
    optimal_used = false;

  }

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
