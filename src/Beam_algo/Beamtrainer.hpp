#ifndef _BEAMTRAINER_
#define _BEAMTRAINER_

#include <vector>
#include <complex>
#include <memory>
#include <utility>
#include <iostream>
#include <global/struct_global.hpp>
#include "common/Beam_util.hpp"

class Beamtrainer{
  protected:
    int ant_num;
    bool isTraining = false;
    bool optimal_used = false;

    std::vector<int> optimalPhaseVector;
    static std::vector<int> curPhaseVector;

  protected:
    std::vector<int> genRandomWeight(void);

  public:
    virtual const std::vector<int> getRespond(struct average_corr_data recvData) = 0;
    virtual const std::vector<int> cannotGetRespond(void) = 0;
    virtual const std::vector<int> startTraining(void) = 0;

    virtual void printClassName(void) = 0;

  public:
    Beamtrainer(int ant_num);
    const std::vector<int> getRandomWeight(void);
    const std::vector<int> getOptimalPhaseVector(void);
    const std::vector<int> getCurPhaseVector(void);

    const bool isOptimalCalculated(void);
    const bool isOptimalUsed(void);
};



#endif
