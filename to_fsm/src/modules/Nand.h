#ifndef NAND_H
#define NAND_H
#include "../Module.h"
#include <vector>
#include "../Wire.h"

class Nand: public Module{

  public:
     Nand(std::vector<Wire*>& inputs_,Wire* output_);
     void do_logic();
};
#endif
