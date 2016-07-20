#include "Module.h"
#include <iostream>

void Module::printInputs(){
  for (Wire*w: inputs){
    std::cout<<w->name<<std::endl;
  }
}
