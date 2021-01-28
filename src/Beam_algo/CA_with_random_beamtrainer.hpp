#ifndef _CA_with_RANDOM_BEAMTRAINER_
#define _CA_with_RANDOM_BEAMTRAINER_

#include <iostream>
#include "Beam_algo/Beamtrainer.hpp"
#include <armadillo>

class CA_with_random_beamtrainer : public Beamtrainer{
  private:
    int training_count = 0;

    arma::Mat<std::complex<float>> trainingWeightMatrix;
    arma::Mat<std::complex<float>> invMatrix;

    arma::Col<std::complex<float>> avgCorrColumn;

    int setNewTrainingVector(std::vector<int> trainingVector);
    int setNewCorrData(std::complex<float> corrData);

    int resetTrainingVector(std::vector<int> trainingVector);
    int resetCorrData(std::complex<float> corrData);

    bool is_processable(void);
    std::vector<int> processOptimalVector(void);

  public:
    CA_with_random_beamtrainer(int ant_num);

    const std::vector<int> startTraining(void);

    const std::vector<int> getRespond(struct average_corr_data recvData);
    const std::vector<int> cannotGetRespond(void);

    void printClassName(void);
};

#endif
