#ifndef PROC_H
#define PROC_H
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


class Processor{


public:
  Processor(std::vector<Wire*>* wires_inputs_,std::vector<Wire*>* wires_ff_,
    std::vector<Wire*>* wires_outputs_,std::vector<Wire*>* wires_others_,
    std::vector<Module*>* modules_,std::vector<Module*>* modules_ff_);

  void processFile(std::vector<std::vector<std::string> >& src_file);


private:
  std::vector<Wire*>* wires_inputs; //enter wires in order encountered
  std::vector<Wire*>* wires_ff;//flip_flop outputs
  std::vector<Wire*>* wires_outputs;
  std::vector<Wire*>* wires_others;
  std::vector<Module*>* modules;
  std::vector<Module*>* modules_ff;
  void processLine(std::vector<std::string>& line);
  Wire* findWireByName(std::string& name);
  enum STATE {INITIAL,SKIP_DFF_MODULE,MAIN_MODULE,DONE};
  enum MAIN_MODULE_SUB_STATE {SCAN,GET_INPUTS,GET_OUTPUTS,GET_OTHERS,GET_MODULES};
  STATE state;
  MAIN_MODULE_SUB_STATE sub_state;

};

#endif
