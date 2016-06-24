#ifndef MODULE_H
#define MODULE_H
#include <vector>
#include "Wire.h"
#include <string>
class Module{

public:
  virtual void do_logic()=0;
  void printInputs();
  std::string name;
  std::vector<Wire*> inputs;
  Wire* output;


};

#endif
