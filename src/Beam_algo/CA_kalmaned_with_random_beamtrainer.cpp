#include "Beam_algo/CA_kalmaned_with_random_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define BaseNum (32)
#define PI  (3.14159265358979323846)

CA_kalmaned_with_random_beamtrainer::CA_kalmaned_with_random_beamtrainer(int ant_num) 
  : Beamtrainer(ant_num)
{
}

void CA_kalmaned_with_random_beamtrainer::printClassName(void){
  std::cout<<"CA_kalmaned_with_random Beamtrainer Selected!"<< std::endl;
}


int CA_kalmaned_with_random_beamtrainer::setNewTrainingVector(std::vector<int> trainingVector)
{
  arma::Row<std::complex<double>> beamWeight(ant_num);

  for(int i = 0; i<ant_num; i++)
  {
    beamWeight(i) = phase2NormalComplex(trainingVector[i]);
  }
  
  if(trainingWeightMatrix.n_rows == ant_num)
  {
    trainingWeightMatrix.shed_row(0);
    trainingWeightMatrix.insert_rows(trainingWeightMatrix.n_rows, beamWeight);
  }else if(trainingWeightMatrix.n_rows < ant_num)
  {
    trainingWeightMatrix.insert_rows(trainingWeightMatrix.n_rows, beamWeight);
  }else
  {
    std::cerr << "Training Weight Matrix Error"<<std::endl;
    exit(1);
  }

  return trainingWeightMatrix.n_rows;
}


int CA_kalmaned_with_random_beamtrainer::setNewCorrData(std::complex<double> corrData)
{
  std::cout << "setNew corr data"<<std::endl;
  arma::Row<std::complex<double>> data(1);
  data(0) = corrData;

  if(avgCorrColumn.n_rows == ant_num)
  {
    avgCorrColumn.shed_row(0);
    avgCorrColumn.insert_rows(avgCorrColumn.n_rows, data);
  }else if(avgCorrColumn.n_rows < ant_num)
  {
    avgCorrColumn.insert_rows(avgCorrColumn.n_rows, data);
  }else
  {
    std::cerr << "Average Correaltion Weight Matrix Error"<<std::endl;
    exit(1);
  }

  return avgCorrColumn.n_rows;
}


int CA_kalmaned_with_random_beamtrainer::resetTrainingVector(std::vector<int> trainingVector)
{
  arma::Row<std::complex<double>> beamWeight(ant_num);

  for(int i = 0; i<ant_num; i++)
  {
    beamWeight(i) = phase2NormalComplex(trainingVector[i]);
  }

  if(trainingWeightMatrix.n_rows <= 0)
  {
    std::cerr << "Tried to reset empty Training Vector"<< std::endl;
    exit(1);
  }else
  {
    trainingWeightMatrix.shed_row(trainingWeightMatrix.n_rows - 1);
    trainingWeightMatrix.insert_rows(trainingWeightMatrix.n_rows, beamWeight);
  }

  return trainingWeightMatrix.n_rows;
}


int CA_kalmaned_with_random_beamtrainer::resetCorrData(std::complex<double> corrData)
{
  arma::Row<std::complex<double>> data(1);
  data(0) = corrData;

  if(avgCorrColumn.n_rows <= 0)
  {
    std::cerr << "Tried to reset empty column"<< std::endl;
    exit(1);
  }else
  {
    avgCorrColumn.shed_row(avgCorrColumn.n_rows - 1);
    avgCorrColumn.insert_rows(avgCorrColumn.n_rows, data);
  }

  return avgCorrColumn.n_rows;
}

std::vector<int> CA_kalmaned_with_random_beamtrainer::processOptimalVector(void)
{
  std::vector<int> phaseVector(ant_num);

  //Check current state is processable
  if(is_processable())
  {
    arma::Mat<std::complex<double>> channelMatrix;
    if(filter == NULL){
      channelMatrix = arma::inv(trainingWeightMatrix) * avgCorrColumn;

      arma::cx_mat   P_0(ant_num, ant_num, arma::fill::eye);
      arma::cx_mat   Q(ant_num, ant_num, arma::fill::eye);
      arma::cx_mat   R   = 0.1 * arma::cx_mat(ant_num, 1, arma::fill::ones);


      filter = new Kalman_filter(ant_num, 
                                channelMatrix, 
                                P_0, 
                                Q,
                                R);
    }else
    {
      channelMatrix = filter->process(avgCorrColumn, arma::inv(trainingWeightMatrix), arma::cx_mat(ant_num, ant_num, arma::fill::eye));
    }

    for (int i = 0; i < ant_num; i++){
      phaseVector[i] = complex2Phase(std::conj(channelMatrix(i)));
    }


  }else
  {
    std::cerr << "Tried to calculate channel with not enough matrix"<< std::endl;
    exit(1);
  }

  return phaseVector;
}

bool CA_kalmaned_with_random_beamtrainer::is_processable(void)
{
  return (trainingWeightMatrix.is_square() && (trainingWeightMatrix.n_rows == avgCorrColumn.n_rows));
}


const std::vector<int> CA_kalmaned_with_random_beamtrainer::startTraining(void){
  //reset all the values
  training_count = 0;
  trainingWeightMatrix.reset();
  isTraining = true;

  curPhaseVector = getRandomWeight();
  setNewTrainingVector(curPhaseVector);

  return curPhaseVector;
}


/*
 *  Handle the tag's respond
 */
const std::vector<int> CA_kalmaned_with_random_beamtrainer::getRespond(struct average_corr_data recvData){
  std::complex<double> corrData(recvData.avg_i, recvData.avg_q);

  if(optimal_used)
  {
    curPhaseVector = getRandomWeight();
    setNewTrainingVector(curPhaseVector);
    optimal_used = false;
  }else
  {
    training_count = setNewCorrData(corrData);
    if(is_processable())
    {
      curPhaseVector = processOptimalVector();
      optimal_used = true;
    }else
    {
      curPhaseVector = getRandomWeight();
      setNewTrainingVector(curPhaseVector);
      optimal_used = false;
    }
  }

  return curPhaseVector;
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> CA_kalmaned_with_random_beamtrainer::cannotGetRespond(void){
  optimal_used = false;
  curPhaseVector = getRandomWeight();
  resetTrainingVector(curPhaseVector);

  return curPhaseVector;
}
