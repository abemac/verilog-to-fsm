#ifndef OR_H
#define OR_H

#include "../Module.h"
#include <vector>
#include "../Wire.h"

class Or : public Module{
public:
  Or(std::vector<Wire*>& inputs_,Wire* output_);
  void do_logic();

};


#endif
