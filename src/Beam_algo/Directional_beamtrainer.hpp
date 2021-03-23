#ifndef _DIRECTIONAL_BEAMTRAINER_
#define _DIRECTIONAL_BEAMTRAINER_

#include <iostream>
#include "Beam_algo/Beamtrainer.hpp"
#include <armadillo>

class Directional_beamtrainer : public Beamtrainer{
  protected:
    int cur_angle_x, cur_angle_y;
    std::vector<int> ant_array;

  protected:
    std::vector<int> getDirectional(int angle_x, int angle_y);
    std::vector<int> getDirectional(int angle);

    std::vector<int> getNextBeam();

    void reset_current_angles();



  public:
    Directional_beamtrainer(int ant_num, std::vector<int> ant_array);

    const std::vector<int> startTraining(void);

    const std::vector<int> getRespond(struct average_corr_data recvData);
    const std::vector<int> cannotGetRespond(void);

    int getBeamNum(void);
    const std::vector<int> beamNum2phaseVec(int beam_num);

    void printClassName(void);
};

#endif
