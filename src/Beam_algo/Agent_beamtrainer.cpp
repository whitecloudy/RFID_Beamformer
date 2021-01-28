#include "Beam_algo/Agent_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>



Agent_beamtrainer::Agent_beamtrainer(int ant_num) : Beamtrainer(ant_num)
{

}

void Agent_beamtrainer::printClassName(void){
    std::cout << "Agent_beamtrainer Selected!!"<<std::endl;
}

const std::vector<int> Agent_beamtrainer::startTraining(void){
    return curPhaseVector;
}

/*
 *  Handle the tag's respond
 */
const std::vector<int> Agent_beamtrainer::getRespond(struct average_corr_data recvData){
    return cannotGetRespond();
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> Agent_beamtrainer::cannotGetRespond(void){
    return curPhaseVector;
}



