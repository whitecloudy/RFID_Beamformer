#ifndef _DIRECTIONAL_BEAMTRAINER_
#define _DIRECTIONAL_BEAMTRAINER_

#include <iostream>
#include "Beam_algo/Beamtrainer.hpp"
#include <armadillo>

class Directional_beamtrainer : public Beamtrainer{
  private:
    int current_angle;

    arma::Mat<std::complex<float>> randomWeightMatrix;
    arma::Mat<std::complex<float>> invMatrix;

    arma::Col<std::complex<float>> avgCorrColumn;

  private:
    std::vector<int> getDirectional(float angle);


  public:
    Directional_beamtrainer(int ant_num);

    const std::vector<int> startTraining(void);

    const std::vector<int> getRespond(struct average_corr_data recvData);
    const std::vector<int> cannotGetRespond(void);
};

#endif
