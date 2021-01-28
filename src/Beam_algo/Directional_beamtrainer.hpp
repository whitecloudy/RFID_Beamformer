#ifndef _DIRECTIONAL_BEAMTRAINER_
#define _DIRECTIONAL_BEAMTRAINER_

#include <iostream>
#include "Beam_algo/Beamtrainer.hpp"
#include <armadillo>

class Directional_beamtrainer : public Beamtrainer{
  protected:
    int current_angle;

  protected:
    std::vector<int> getDirectional(double angle);


  public:
    Directional_beamtrainer(int ant_num);

    const std::vector<int> startTraining(void);

    const std::vector<int> getRespond(struct average_corr_data recvData);
    const std::vector<int> cannotGetRespond(void);

    void printClassName(void);
};

#endif
