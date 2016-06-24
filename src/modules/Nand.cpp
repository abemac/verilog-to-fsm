#include "Nand.h"

Nand::Nand(std::vector<Wire*>& inputs_,Wire* output_){
  for(Wire* w : inputs_){
    inputs.push_back(w);
  }
  output=output_;

}
void Nand::do_logic(){
    int tmp = inputs[0]->val && inputs[1]->val;
    for(unsigned int i=2;i<inputs.size();i++){
      tmp&=inputs[i]->val;
    }
    output->val=!tmp;
}
