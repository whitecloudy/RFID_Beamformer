#ifndef _ADAPTIVE_BEAMTRAINER_
#define _ADAPTIVE_BEAMTRAINER_

#include <iostream>
#include "Beam_algo/Beamtrainer.hpp"
#include <armadillo>

class Random_beamtrainer : public Beamtrainer{
  private:
    int training_count = 0;

    arma::Mat<std::complex<float>> randomWeightMatrix;
    arma::Mat<std::complex<float>> invMatrix;

    arma::Col<std::complex<float>> avgCorrColumn;


  private:
    arma::Row<std::complex<float>> generateRandomWeight(int rowSize);

  public:
    Random_beamtrainer(int ant_num);

    const std::vector<int> startTraining(void);

    const std::vector<int> getRespond(struct average_corr_data recvData);
    const std::vector<int> cannotGetRespond(void);
};

#endif
