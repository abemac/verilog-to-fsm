#ifndef NOT_H
#define NOT_H
#include "../Module.h"
#include "../Wire.h"
#include <vector>

class Not : public Module{

public:
  Not(Wire* input_,Wire* output_);
  void do_logic();


};

#endif
