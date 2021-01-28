#include <iostream>
#include <lib/AD5371_ctrl/Phase_Attenuator_controller.hpp>
#include <lib/cxxopts/include/cxxopts.hpp>

#include "global/struct_global.hpp"

#include "Beam_algo/Beam_algorithm.hpp"
#include "Beamformer.hpp"
#include <cstdlib>
#include <ctime>

static struct args_struct{
  bool                    debug = false;
  int                     sic_ant_num = -1;
  std::vector<int>        ant_nums;
  BEAM_ALGO::algorithm    beam_algo;
} args;


int parse_arg(int argc, char ** argv){
  cxxopts::Options options("Beamforming program", "Program that handles the beam with USRP and Raspberry pi and 16way board");
  options.add_options()
    ("d,debug", "Enable debugging")
    ("a,ant", "antenna number", cxxopts::value<std::vector<int>>())
    ("s,sic", "Self-interference Cancellation",cxxopts::value<int>()->default_value("-1"))
    ("A,algo", "Beamforming Algorithm", cxxopts::value<std::string>())
    ("h,help", "Print usage")
    ;

  auto opt_result = options.parse(argc,argv);

  if(opt_result.count("help")){
    std::cout << options.help() << std::endl;
    exit(0);
  }

  args.debug       = opt_result["d"].as<bool>();
  args.sic_ant_num = opt_result["s"].as<int>();
  if((args.ant_nums    = opt_result["a"].as<std::vector<int>>()).size() == 0)
    exit(1);
  args.beam_algo = BEAM_ALGO::parse_beam_algorithm(opt_result["A"].as<std::string>());
  
  return 0;
}


int main(int argc, char ** argv) {

  //int ant_amount = argc - 1;
  //int * ant_nums = new int[ant_amount];

  if(parse_arg(argc, argv))
    return 1;

  //do test beamforming
  Beamformer beamformer(args.ant_nums, args.beam_algo, args.sic_ant_num);

  if(beamformer.start_beamformer()){
    std::cout<< "Error : beamformer has terminated with error"<< std::endl;
    return 1;
  }
  
  return 0;
}
