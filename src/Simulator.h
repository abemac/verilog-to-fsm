#ifndef SIM_H
#define SIM_H
#include <vector>
#include <string>
#include "Wire.h"
#include "modules/And.h"
#include "modules/Not.h"
#include "modules/Or.h"
#include "modules/Nand.h"
#include "modules/Nor.h"
#include "modules/Dff.h"
#include "Module.h"
#include <fstream>

class Simulator{
public:
  Simulator(std::vector<Wire*>* wires_inputs_,std::vector<Wire*>* wires_ff_,
      std::vector<Wire*>* wires_outputs_,std::vector<Wire*>* wires_others_,
      std::vector<Module*>* modules_,std::vector<Module*>* modules_ff_);
  void simulate();
private:
  std::vector<Wire*>* wires_inputs; //enter wires in order encountered
  std::vector<Wire*>* wires_ff;//flip_flop outputs
  std::vector<Wire*>* wires_outputs;
  std::vector<Wire*>* wires_others;
  std::vector<Module*>* modules;
  std::vector<Module*>* modules_ff;
  unsigned long long inputs;
  unsigned long long flip_flops;
  void assign_wires();
  void do_logic();

};

#endif
