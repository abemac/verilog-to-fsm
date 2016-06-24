#ifndef DFF_H
#define DFF_H

#include "../Module.h"
#include "../Wire.h"
#include <vector>

class Dff : public Module{

public:
  Dff(Wire* D, Wire *Q);
  void do_logic();

};

#endif
