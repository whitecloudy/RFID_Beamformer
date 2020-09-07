#ifndef _BEAMTRAINER_
#define _BEAMTRAINER_

#include <vector>
#include <complex>
#include "struct_global.hpp"

#define PI  (3.14159265358979323846)

class Beamtrainer{
  protected:
    int ant_num;
    bool isTraining = false;
    std::vector<int> optimalPhaseVector;

  protected:
    static float complex2Phase(std::complex<float> complexData);
    static std::complex<float> phase2NormalComplex(float phaseData);
    static std::vector<int> genRandomWeight(void);

  public:
    virtual const std::vector<int> getRespond(struct average_corr_data recvData) = 0;
    virtual const std::vector<int> cannotGetRespond(void) = 0;
    virtual const std::vector<int> startTraining(void) = 0;

  public:
    Beamtrainer(int ant_num);
    const std::vector<int> getRandomWeight(void);
    const bool isOptimalCalculated(void);
    const std::vector<int> getOptimalPhaseVector(void);
};

#endif
