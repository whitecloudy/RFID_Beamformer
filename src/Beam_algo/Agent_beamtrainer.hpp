#ifndef _AGENT_BEAMTRAINER_
#define _AGENT_BEAMTRAINER_

#include <iostream>
#include "Beam_algo/Beamtrainer.hpp"
#include "Beam_algo/Directional_beamtrainer.hpp"
#include <armadillo>
#include <random>
#include <memory>
#include <list>
#include "common/CA_calculator.hpp"
#include "common/Kalmaned_CA_calculator.hpp"
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define MAXIMUM_DATA (4092)

class Agent_communicator{
  private:
    int sockfd = 0; 
    struct sockaddr_in     servaddr, cliaddr;

    char * buf;

    int counter=0;
    int max_num;
    bool opt_flag = false;

    std::vector<int> nn_opt_data, heur_opt_data;


  public:
    Agent_communicator(int num);
    ~Agent_communicator();
  
    int send_data(std::vector<int> phase_vec, float tag_i, float tag_q, float noise_i, float noise_q);
    std::vector<std::complex<float>> recv_data(void);
    int reset(void);
    bool is_opt_ready(void);

    std::vector<int> get_nn_opt(void);
    std::vector<int> get_heur_opt(void);

    int send_end(void);
};

class Agent_beamtrainer : public Directional_beamtrainer{
  private:
    int best_beam_count;
    int best_beam_max;
    int round_count;
    int round_max;

    bool beamSearchFlag = true;

    Agent_communicator comm;

    struct beamStruct
    {
      int beamNum;
      double amp;
    };
    std::list<struct beamStruct> rankBeamL;
    std::vector<int> bestBeamNum;

    std::vector<int> curCenterPhaseVector;

    void reset_Agent_beamtrainer(void);

  private:
    std::vector<int> randomScramble(std::vector<int> center, double std);

  public:
    Agent_beamtrainer(int ant_num, std::vector<int> ant_array, int actual_round, int round_max=5);

    const std::vector<int> startTraining(void);
    const std::vector<int> getRespond(struct average_corr_data recvData, std::vector<int> usedVector = curPhaseVector);
    const std::vector<int> cannotGetRespond(std::vector<int> usedVector = curPhaseVector);

    void printClassName(void);
};

#endif
