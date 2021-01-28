#ifndef _AGENT_BEAMTRAINER_
#define _AGENT_BEAMTRAINER_

#include <iostream>
#include "Beam_algo/Beamtrainer.hpp"
#include <armadillo>


class Agent_beamtrainer : public Beamtrainer{
  private:


  public:
    Agent_beamtrainer(int ant_num);

    const std::vector<int> startTraining(void);

    const std::vector<int> getRespond(struct average_corr_data recvData);
    const std::vector<int> cannotGetRespond(void);

    void printClassName(void);
};

#endif
