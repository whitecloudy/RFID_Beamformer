#include "Beam_algo/Fixed_beamtrainer.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

#define __BEAM_ANGLE_RANGE  (90)
#define __BEAM_ANGLE_STEP   (15)

Fixed_beamtrainer::Fixed_beamtrainer(int ant_num) : Beamtrainer(ant_num){
    for(int i = 0; i < ant_num; i++){
        std::cout << "Phase ant "<<i<<" : ";
        std::cin >> curPhaseVector[i];
        std::cout << std::endl;
    }
}

void Fixed_beamtrainer::printClassName(void){
    std::cout << "Fixed_beamtrainer Selected!!"<<std::endl;
}

const std::vector<int> Fixed_beamtrainer::startTraining(void){
    return curPhaseVector;
}

/*
 *  Handle the tag's respond
 */
const std::vector<int> Fixed_beamtrainer::getRespond(struct average_corr_data recvData,std::vector<int> usedVector){
    return cannotGetRespond();
}

/*
 * Handle when the tag does not respond
 */
const std::vector<int> Fixed_beamtrainer::cannotGetRespond(std::vector<int> usedVector){
    return curPhaseVector;
}



