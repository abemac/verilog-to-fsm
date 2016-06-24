#ifndef WIRE_H
#define WIRE_H
#include <string>

class Wire{



public:
  Wire(std::string name);

  int val;
  bool stable;
  std::string name;



};

#endif
