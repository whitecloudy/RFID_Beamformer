#ifndef _Kalmaned_with_RANDOM_BEAMTRAINER_
#define _Kalmaned_with_RANDOM_BEAMTRAINER_

#include <iostream>
#include "Beam_algo/Beamtrainer.hpp"
#include "common/Kalmaned_CA_calculator.hpp"
#include <armadillo>

class Kalmaned_with_random_beamtrainer : public Beamtrainer{
  private:
    int training_count = 0;

    Kalmaned_CA_calculator ca_cal;


  public:
    Kalmaned_with_random_beamtrainer(int ant_num);

    const std::vector<int> startTraining(void);
    const std::vector<int> getRespond(struct average_corr_data recvData);
    const std::vector<int> cannotGetRespond(void);

    void printClassName(void);
};

#endif
