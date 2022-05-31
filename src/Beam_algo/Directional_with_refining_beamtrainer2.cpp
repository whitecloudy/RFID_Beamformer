#include "Beam_algo/Directional_with_refining_beamtrainer2.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define __SCRAMBLE_VAR      (30)

Directional_with_refining_beamtrainer2::Directional_with_refining_beamtrainer2(int ant_num, std::vector<int> ant_array, int training_frame) : Directional_beamtrainer(ant_num, ant_array), curCenterPhaseVector(ant_num), ca_cal(ant_num, 0){

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

void Directional_with_refining_beamtrainer2::printClassName(void){
  std::cout << "Directional_with_refining_beamtrainer2 Selected!!"<<std::endl;
}

const std::vector<int> Directional_with_refining_beamtrainer2::startTraining(void){
  //reset all the values
  reset_Directional_with_refining_beamtrainer();

  isTraining = true;

  trainingPhaseVector = getDirectional(cur_angle_x, cur_angle_y);
  curPhaseVector = trainingPhaseVector;

  return curPhaseVector;
}


void Directional_with_refining_beamtrainer2::reset_Directional_with_refining_beamtrainer(void)
{
  reset_Directional_beamtrainer();

  ca_cal.clear();

  bestBeam.x = 0;
  bestBeam.y = 0;
  bestBeam.amp = 0;
  bestBeamFlag = false;

  curCenterPhaseVector.clear();
  beamSearchFlag = true;
}



/*
 *  Handle the tag's respond
 */
const std::vector<int> Directional_with_refining_beamtrainer2::getRespond(struct average_corr_data recvData, std::vector<int> usedVector){
  if(!optimal_used)
  {
    ca_cal.setNewTrainingVector(usedVector);
    ca_cal.setNewCorrData(std::complex<double>(recvData.avg_i, recvData.avg_q));
  }

  if(beamSearchFlag)
  {

    if(bestBeam.amp < recvData.avg_corr)
    {
      bestBeam.x = cur_angle_x;
      bestBeam.y = cur_angle_y;
      bestBeam.amp = recvData.avg_corr;
      bestBeamFlag = true;

      //optimalPhaseVector = curPhaseVector;
      //optimal_available = true;
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
    if(bestBeam.amp < recvData.avg_corr)
    {
      bestBeam.x = cur_angle_x;
      bestBeam.y = cur_angle_y;
      bestBeam.amp = recvData.avg_corr;
      bestBeamFlag = true;

      //optimalPhaseVector = curPhaseVector;
      //optimal_available = true;
    }

    if(ca_cal.is_processable() && !optimal_used)
    {
      optimalPhaseVector[0] = ca_cal.processOptimalVector();
      optimal_available = true;
    }

    trainingPhaseVector = getNextRefiningBeam();
  }

  optimal_used = false;
  curPhaseVector = trainingPhaseVector;
  return curPhaseVector;
}

const std::vector<int> Directional_with_refining_beamtrainer2::reset_refining_beam_angles(void)
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

const std::vector<int> Directional_with_refining_beamtrainer2::getNextRefiningBeam(void)
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
const std::vector<int> Directional_with_refining_beamtrainer2::cannotGetRespond(std::vector<int> usedVector){
  if(beamSearchFlag)
  {
    trainingPhaseVector = getNextBeam();

    int beamNum = getBeamNum();

    if((beamNum == 0)&&(bestBeamFlag))
    {
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
