#include "Not.h"

Not::Not(Wire* input_,Wire* output_){
  inputs.push_back(input_);
  output=output_;

}
void Not::do_logic(){
    output->val=!inputs[0]->val;
}
