#include "Simulator.h"
#include <iostream>
Simulator::Simulator(std::vector<Wire*>* wires_inputs_,std::vector<Wire*>* wires_ff_,
  std::vector<Wire*>* wires_outputs_,std::vector<Wire*>* wires_others_,
  std::vector<Module*> * modules_,std::vector<Module*>* modules_ff_){
      wires_inputs=wires_inputs_;
      wires_ff=wires_ff_;
      wires_outputs=wires_outputs_;
      wires_others=wires_others_;
      modules=modules_;
      modules_ff=modules_ff_;
      flip_flops=0;
      inputs=0;

  }

  void Simulator::simulate(){
    if(wires_ff->size()>64 || wires_inputs->size()>64){
      std::cout<<"TOO BIG"<<std::endl;
      return;
    }
    unsigned long long flip_flop_upper_limit =(( unsigned long long )(-1)>>(64-wires_ff->size()));
    unsigned long long inputs_upper_limit =(( unsigned long long )(-1)>>(64-wires_inputs->size()));
    std::ofstream FILE;
    FILE.open("output.csv");
    FILE<<"current state,input,output,next state\n";
    // std::cout<<flip_flops<<std::endl;
    while(flip_flops<=flip_flop_upper_limit){

      inputs=0;
      assign_wires();
      while(inputs<=inputs_upper_limit){
        std::cout<<"\e[A**Now simulating circuit... ("<<flip_flops<<"/"<<flip_flop_upper_limit<<"), ("<<
            inputs<<"/"<<inputs_upper_limit<<")"<<std::endl;
        for(Wire*w : *wires_others){
          w->stable=false;
        }
        for(Wire*w : *wires_outputs){
          w->stable=false;
        }

        do_logic();
        //get flip flop inputs as next state (with current input)
        FILE<<flip_flops<<","<<inputs<<",";
        unsigned long long output=0;
        for(Wire * w : *(wires_outputs)){
          output<<=1;
          output|=w->val;
        }
        FILE<<output<<",";
        unsigned long long next_state=0;
        for(Module* m : *(modules_ff)){
          next_state<<=1;
          next_state|=m->inputs[0]->val;
        }
        FILE<<next_state<<"\n";

        inputs++;
        assign_wires();

      }
      flip_flops++;
      assign_wires();
    }

    FILE.close();

  }



  void Simulator::assign_wires(){
    for(unsigned int i=0;i<wires_inputs->size();i++){
      wires_inputs->operator[](i)->val=(inputs >> (wires_inputs->size()-1-i)) & 1;
      wires_inputs->operator[](i)->stable=true;
    }
    for(unsigned int i=0;i<wires_ff->size();i++){
      wires_ff->operator[](i)->val=(flip_flops >> (wires_ff->size()-1-i)) & 1;
      wires_ff->operator[](i)->stable=true;
    }
  }

  void Simulator::do_logic(){
    std::vector<Module*> toDo;

    for(Module* m : *(modules)){

      bool do_logic=true;
      for(Wire* w : m->inputs){
        //std::cout<<w->stable<<" "<<w->name<<std::endl;
        if(w->stable==false){
          do_logic=false;
        }
      }
      if(do_logic){
        m->do_logic();
        m->output->stable=true;
      }else{
        toDo.push_back(m);
      }
    }

    while(toDo.size()>0){
      bool do_logic =false;
      unsigned int i=0;

      while(do_logic==false && toDo.size()>0 && i<toDo.size()){
        do_logic=true;
        for(Wire* w : toDo[i]->inputs){
          if(w->stable==false){
            do_logic=false;
          }
        }
        //std::cout<<i<<std::endl;
        if(do_logic){
          toDo[i]->do_logic();
          toDo[i]->output->stable=true;
          toDo.erase(toDo.begin()+i);
        }else{
          i++;
        }
      }
    }

  }
