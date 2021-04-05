#ifndef _CA_with_RANDOM_BEAMTRAINER_
#define _CA_with_RANDOM_BEAMTRAINER_

#include <iostream>
#include "Beamtrainer.hpp"
#include "common/CA_calculator.hpp"
#include <armadillo>

class CA_with_random_beamtrainer : public Beamtrainer{
  private:
    int training_count = 0;

    CA_calculator ca_cal;

  public:
    CA_with_random_beamtrainer(int ant_num);

    const std::vector<int> startTraining(void);
    const std::vector<int> getRespond(struct average_corr_data recvData, std::vector<int> usedVector = curPhaseVector);
    const std::vector<int> cannotGetRespond(std::vector<int> usedVector = curPhaseVector);

    void printClassName(void);
};

#endif
