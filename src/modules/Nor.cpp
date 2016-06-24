#include "Nor.h"

Nor::Nor(std::vector<Wire*>& inputs_,Wire* output_){
  for(Wire* w : inputs_){
    inputs.push_back(w);
  }
  output=output_;

}
void Nor::do_logic(){
    for(Wire* w : inputs){
      if(w->val==1){
        output->val=0;
        return;
      }
    }
    output->val=1;

}
