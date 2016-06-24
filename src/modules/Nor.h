#ifndef NOR_H
#define NOR_H

#include "../Module.h"
#include <vector>
#include "../Wire.h"

class Nor : public Module{
public:
  Nor(std::vector<Wire*>& inputs_,Wire* output_);
  void do_logic();

};


#endif
