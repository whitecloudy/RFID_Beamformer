#ifndef _DIRECTIONAL_BEAMTRAINER_
#define _DIRECTIONAL_BEAMTRAINER_

#include <iostream>
#include "Beam_algo/Beamtrainer.hpp"
#include <armadillo>

#define __BEAM_X_ANGLE_RANGE  (60)
#define __BEAM_Y_ANGLE_RANGE  (30)
#define __BEAM_ANGLE_STEP   (15)


class Directional_beamtrainer : public Beamtrainer{
  protected:
    int cur_angle_x, cur_angle_y;
    std::vector<int> ant_array;

  protected:
    std::vector<int> getDirectional(int angle_x, int angle_y);
    std::vector<int> getDirectional(int angle);

    std::vector<int> getNextBeam();
    std::vector<int> getCurBeam();

    void reset_current_angles();

    void reset_Directional_beamtrainer(void);



  public:
    Directional_beamtrainer(int ant_num, std::vector<int> ant_array);

    const std::vector<int> startTraining(void);

    const std::vector<int> getRespond(struct average_corr_data recvDatam, std::vector<int> usedVector = curPhaseVector);
    const std::vector<int> cannotGetRespond(std::vector<int> usedVector = curPhaseVector);

    int getBeamNum(void);
    const std::vector<int> beamNum2phaseVec(int beam_num);

    void printClassName(void);
};

#endif
