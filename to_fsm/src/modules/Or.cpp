#include "Or.h"

Or::Or(std::vector<Wire*>& inputs_,Wire* output_){
  for(Wire* w : inputs_){
    inputs.push_back(w);
  }
  output=output_;

}
void Or::do_logic(){
    for(Wire* w : inputs){
      if(w->val==1){
        output->val=1;
        return;
      }
    }
    output->val=0;

}
