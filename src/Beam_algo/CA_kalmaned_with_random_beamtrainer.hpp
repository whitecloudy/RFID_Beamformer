#ifndef _CA_kalmaned_with_RANDOM_BEAMTRAINER_
#define _CA_kalmaned_with_RANDOM_BEAMTRAINER_

#include <iostream>
#include "Beam_algo/Beamtrainer.hpp"
#include <lib/kalman_filter/Kalman_filter.hpp>
#include <armadillo>

class CA_kalmaned_with_random_beamtrainer : public Beamtrainer{
  private:
    int training_count = 0;
    Kalman_filter * filter = NULL;

    arma::Mat<std::complex<double>> trainingWeightMatrix;
    arma::Mat<std::complex<double>> invMatrix;

    arma::Col<std::complex<double>> avgCorrColumn;

    int setNewTrainingVector(std::vector<int> trainingVector);
    int setNewCorrData(std::complex<double> corrData);

    int resetTrainingVector(std::vector<int> trainingVector);
    int resetCorrData(std::complex<double> corrData);

    bool is_processable(void);
    std::vector<int> processOptimalVector(void);

  public:
    CA_kalmaned_with_random_beamtrainer(int ant_num);

    const std::vector<int> startTraining(void);

    const std::vector<int> getRespond(struct average_corr_data recvData);
    const std::vector<int> cannotGetRespond(void);

    void printClassName(void);
};

#endif
