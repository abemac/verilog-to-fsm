#ifndef AND_H
#define AND_H
#include "../Module.h"
#include <vector>
#include "../Wire.h"

class And: public Module{

  public:
     And(std::vector<Wire*>& inputs_,Wire* output_);
     void do_logic();
};
#endif
