#ifndef _FIXED_BEAMTRAINER_
#define _FIXED_BEAMTRAINER_

#include <iostream>
#include "Beam_algo/Beamtrainer.hpp"
#include <armadillo>

class Fixed_beamtrainer : public Beamtrainer{
  protected:
    int current_angle;

  public:
    Fixed_beamtrainer(int ant_num);

    const std::vector<int> startTraining(void);

    const std::vector<int> getRespond(struct average_corr_data recvData);
    const std::vector<int> cannotGetRespond(void);

    void printClassName(void);
};

#endif
