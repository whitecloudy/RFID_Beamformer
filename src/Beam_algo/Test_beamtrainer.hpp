#ifndef _TEST_BEAMTRAINER_
#define _TEST_BEAMTRAINER_

#include <iostream>
#include "Beam_algo/Beamtrainer.hpp"
#include "common/CA_calculator.hpp"
#include "common/Kalmaned_CA_calculator.hpp"
#include <armadillo>

class Test_beamtrainer : public Beamtrainer{
  private:
    int training_count = 0;

    //Kalmaned_CA_calculator  kalman_ca_cal;
    bool kalman_optimal   = false;
    CA_calculator           ca_cal;
    CA_calculator           kalman_ca_cal;

    bool ca_optimal          = false;


  public:
    Test_beamtrainer(int ant_num);

    const std::vector<int> startTraining(void);
    const std::vector<int> getRespond(struct average_corr_data recvData, std::vector<int> usedVector = curPhaseVector);
    const std::vector<int> cannotGetRespond(std::vector<int> usedVector = curPhaseVector);

    const int which_optimal(void);
    void printClassName(void);
};

#endif
