#include "Dff.h"


Dff::Dff(Wire* D, Wire* Q){
  inputs.push_back(D);
  output = Q;
}

void Dff::do_logic(){
  output->val=inputs[0]->val;
}
