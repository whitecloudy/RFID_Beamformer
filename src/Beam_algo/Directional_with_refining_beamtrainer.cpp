#include "Beam_algo/Directional_with_refining_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define __SCRAMBLE_VAR      (30)

Directional_with_refining_beamtrainer::Directional_with_refining_beamtrainer(int ant_num, std::vector<int> ant_array, int training_frame) : Directional_beamtrainer(ant_num, ant_array), curCenterPhaseVector(ant_num){

  this->training_frame = training_frame;

  if((training_frame-27)/3 > 3)
  {
    x_step = (training_frame-27)/3;
    y_step = 3;
  }else
  {
    x_step = 3;
    y_step = (training_frame-27)/3;
  }

}

void Directional_with_refining_beamtrainer::printClassName(void){
  std::cout << "Directional_with_refining_beamtrainer Selected!!"<<std::endl;
}

const std::vector<int> Directional_with_refining_beamtrainer::startTraining(void){
  //reset all the values
  reset_Directional_with_refining_beamtrainer();

  isTraining = true;

  trainingPhaseVector = getDirectional(cur_angle_x, cur_angle_y);
  curPhaseVector = trainingPhaseVector;

  return curPhaseVector;
}


void Directional_with_refining_beamtrainer::reset_Directional_with_refining_beamtrainer(void)
{
  reset_Directional_beamtrainer();

  bestBeam.x = 0;
  bestBeam.y = 0;
  bestBeam.amp = std::complex<double>(0.0,0.0);
  bestBeamFlag = false;

  curCenterPhaseVector.clear();
  beamSearchFlag = true;
}



/*
 *  Handle the tag's respond
 */
const std::vector<int> Directional_with_refining_beamtrainer::getRespond(struct average_corr_data recvData, std::vector<int> usedVector){

  if(beamSearchFlag)
  {
    if(std::abs(bestBeam.amp) < recvData.avg_corr)
    {
      bestBeam.x = cur_angle_x;
      bestBeam.y = cur_angle_y;
      bestBeam.amp = std::complex<double>(recvData.avg_i, recvData.avg_q);
      bestBeamFlag = true;
      
      optimalPhaseVector = beamPhaseShift(std::conj(bestBeam.amp), curPhaseVector);
      optimal_available = true;
    }

    trainingPhaseVector = getNextBeam();

    int beamNum = getBeamNum();

    if((beamNum == 0)&&(bestBeamFlag))
    {
      cur_center_x = bestBeam.x;
      cur_center_y = bestBeam.y;

      curCenterPhaseVector = getDirectional(bestBeam.x, bestBeam.y);
      trainingPhaseVector  = reset_refining_beam_angles();

      beamSearchFlag = false;
    }
  }else
  {
    if(std::abs(bestBeam.amp) < recvData.avg_corr)
    {
      bestBeam.x = cur_angle_x;
      bestBeam.y = cur_angle_y;
      bestBeam.amp = std::complex<double>(recvData.avg_i, recvData.avg_q);
      bestBeamFlag = true;

      optimalPhaseVector = beamPhaseShift(std::conj(bestBeam.amp), curPhaseVector);
      optimal_available = true;
    }
    trainingPhaseVector = getNextRefiningBeam();
  }

  optimal_used = false;
  curPhaseVector = trainingPhaseVector;
  return curPhaseVector;
}

const std::vector<int> Directional_with_refining_beamtrainer::reset_refining_beam_angles(void)
{
  round_count = 0;

  if(x_step > 1)
    cur_refine_x = cur_center_x - (double)__BEAM_X_ANGLE_STEP/2;
  else
    cur_refine_x = cur_center_x;

  if(y_step > 1)
    cur_refine_y = cur_center_y - (double)__BEAM_Y_ANGLE_STEP/2;
  else
    cur_refine_y = cur_center_y;

  std::cout << cur_refine_x << ", "<<cur_refine_y << std::endl;
  
  return getDirectional(cur_refine_x, cur_refine_y);

}

const std::vector<int> Directional_with_refining_beamtrainer::getNextRefiningBeam(void)
{
  round_count+=1;

  if(x_step > 1)
  {
    double x_angle_step = (double)__BEAM_X_ANGLE_STEP/(x_step - 1);
    cur_refine_x = cur_center_x - (double)__BEAM_X_ANGLE_STEP/2 + x_angle_step * (round_count % x_step);
  }else
    cur_refine_x = cur_center_x;


  if(y_step > 1)
  {
    double y_angle_step = (double)__BEAM_Y_ANGLE_STEP/(y_step - 1);
    cur_refine_y = cur_center_y - (double)__BEAM_Y_ANGLE_STEP/2 + y_angle_step * ((round_count / x_step) % y_step);
  }else
    cur_refine_y = cur_center_y;

  std::cout << cur_refine_x << ", "<<cur_refine_y << std::endl;

  return getDirectional(cur_refine_x, cur_refine_y);
}


/*
 * Handle when the tag does not respond
 */
const std::vector<int> Directional_with_refining_beamtrainer::cannotGetRespond(std::vector<int> usedVector){
  if(beamSearchFlag)
  {
    trainingPhaseVector = getNextBeam();

    int beamNum = getBeamNum();

    if((beamNum == 0)&&(bestBeamFlag))
    {
      optimal_available = true;

      cur_center_x = bestBeam.x;
      cur_center_y = bestBeam.y;

      curCenterPhaseVector = getDirectional(bestBeam.x, bestBeam.y);
      trainingPhaseVector  = reset_refining_beam_angles();;

      beamSearchFlag = false;
    }
  }else
  {
    trainingPhaseVector = getNextRefiningBeam();
  }

  optimal_used = false;
  curPhaseVector = trainingPhaseVector;
  return curPhaseVector;
}
