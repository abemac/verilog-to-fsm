#include "Wire.h"

Wire::Wire(std::string name_){
  val=0;
  name=name_;
  stable=false;
}
