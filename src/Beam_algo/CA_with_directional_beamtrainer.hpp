#ifndef _CA_with_DIRECTIONAL_BEAMTRAINER_
#define _CA_with_DIRECTIONAL_BEAMTRAINER_

#include <iostream>
#include "Beam_algo/Beamtrainer.hpp"
#include "Beam_algo/Directional_beamtrainer.hpp"
#include <armadillo>
#include <random>
#include <memory>

class CA_with_directional_beamtrainer : public Directional_beamtrainer{
  private:
    int round_count;
    int round_max;

    std::vector<int> curCenterPhaseVector;

  private:
    std::vector<int> randomScramble(std::vector<int> center, float std);

  public:
    CA_with_directional_beamtrainer(int ant_num, int round_max=5);

    const std::vector<int> startTraining(void);

    const std::vector<int> getRespond(struct average_corr_data recvData);
    const std::vector<int> cannotGetRespond(void);

    void printClassName(void);
};

#endif
