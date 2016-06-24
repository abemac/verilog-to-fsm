#include "Processor.h"
#include <iostream>

Processor::Processor(std::vector<Wire*>* wires_inputs_,std::vector<Wire*>* wires_ff_,
  std::vector<Wire*>* wires_outputs_,std::vector<Wire*>* wires_others_,
  std::vector<Module*> * modules_,std::vector<Module*>* modules_ff_){
      wires_inputs=wires_inputs_;
      wires_ff=wires_ff_;
      wires_outputs=wires_outputs_;
      wires_others=wires_others_;
      modules=modules_;
      modules_ff=modules_ff_;
      state = INITIAL;
      sub_state=SCAN;
  }


void Processor::processFile(std::vector<std::vector<std::string> >& src_file){
  double a=0;
  
  for(std::vector<std::string> v: src_file){
      processLine(v);
      a++;
      int progress=(a/(src_file.size()))*100;
      std::cout<<"\e[AGenerating data structures for Simulation... ("<<progress<<"\% done)"<<std::endl;
  }


}


void Processor::processLine(std::vector<std::string>& line){
  if(line.size()==0){return;}
  if(line[0].size()>=2 && (line[0].substr(0,2).compare("//")==0)){return;}
  // for(std::string s: line){
  //   std::cout<<s<<" ";
  // }std::cout<<""<<std::endl;
  if(state==INITIAL){
    if(line[0].compare("module")==0){
      if(line[1].compare("dff")==0){
        state=SKIP_DFF_MODULE;
      }else{
        state=MAIN_MODULE;
      }
    }
    return;
  }
  else if (state==SKIP_DFF_MODULE){
    if(line[0].compare("endmodule")==0){
      state=INITIAL;
    }
    return;
  }
  else if (state==MAIN_MODULE){
    //std::cout<<"working"<<std::endl;
    //std::cout<<line[0]<<"  "<<(line[0].compare("wire")==0)<<" state="<<sub_state<<std::endl;
    if(sub_state==SCAN || sub_state==GET_MODULES){
      if(line[0].compare("input")==0){
        sub_state=GET_INPUTS;
      }else if(line[0].compare("output")==0){
        sub_state=GET_OUTPUTS;
      }else if(line[0].compare("wire")==0){
        sub_state=GET_OTHERS;
      }else{
        sub_state=GET_MODULES;
      }
    }
    if(sub_state==GET_INPUTS){
      unsigned int i=0;
      int index=0;
      if(line[0].compare("input")==0){index++;}
      std::string name;
      while(i<line[index].size()){
        if(line[index][i]==','){
          if(name.compare("GND") && name.compare("VDD") && name.compare("CK")){
            wires_inputs->push_back(new Wire(name));
          }
          name.clear();
        }else if(line[index][i]==';'){
          if(name.compare("GND") && name.compare("VDD") && name.compare("CK")){
            wires_inputs->push_back(new Wire(name));
          }
          sub_state=SCAN;
        }
        else{
          name.push_back(line[index][i]);
        }
        i++;
      }

    }
    if(sub_state==GET_OUTPUTS){
      unsigned int i=0;
      int index=0;
      if(line[0].compare("output")==0){index++;}
      std::string name;
      while(i<line[index].size()){
        if(line[index][i]==','){
          wires_outputs->push_back(new Wire(name));
          name.clear();
        }else if(line[index][i]==';'){
          wires_outputs->push_back(new Wire(name));
          sub_state=SCAN;
        }
        else{
          name.push_back(line[index][i]);
        }
        i++;
      }
    }
    if(sub_state==GET_OTHERS){

      unsigned int i=0;
      int index=0;
      if(line[0].compare("wire")==0){index++;}
      std::string name;
      while(i<line[index].size()){
        if(line[index][i]==','){
          wires_others->push_back(new Wire(name));
          name.clear();
        }else if(line[index][i]==';'){
          wires_others->push_back(new Wire(name));
          sub_state=SCAN;
        }
        else{
          name.push_back(line[index][i]);
        }
        i++;
      }
    }
    if(sub_state==GET_MODULES){

      if(line[0].compare("dff")==0){
        unsigned int i=0;
        unsigned int paramNum=0;
        std::string buf;
        std::string name;
        Wire* d;
        Wire* q;
        while(i<line[1].size()){
          //std::cout<<"here ("<<i<<"),  buf="<<buf<<", line="<<line[1]<<std::endl;
          if(line[1][i]=='('){
            name.assign(buf);
            buf.clear();
          }
          else if(line[1][i]==',' || line[1][i]==')'){
            if(paramNum==0){//CK-ignore
              paramNum++;
              buf.clear();
            }else if (paramNum==1){//output (Q)
              paramNum++;
              q=findWireByName(buf);
              wires_ff->push_back(q);
              //need to remove from "others"
              bool removed=false;
              for(unsigned int i=0;i<wires_others->size();i++){
                if(wires_others->operator[](i)==q){
                  wires_others->erase(wires_others->begin()+i);
                  removed=true;
                }
              }
              if(!removed){
                for(unsigned int i=0;i<wires_outputs->size();i++){
                  if(wires_outputs->operator[](i)==q){
                    wires_outputs->erase(wires_outputs->begin()+i);
                  }
                }
              }

              buf.clear();
            }else if (paramNum==2){//input D
              paramNum++;
              d=findWireByName(buf);
              buf.clear();
            }

          }else{
            buf.push_back(line[1][i]);
          }
          i++;
        }

        modules_ff->push_back(new Dff(d,q));
        modules_ff->back()->name.assign(name);

      }else if (line[0].compare("and")==0){
        unsigned int i=0;
        unsigned int paramNum=0;
        std::string buf;
        std::string name;
        std::vector<Wire*> inputs;
        Wire* output;
        while(i<line[1].size()){
          //std::cout<<"here ("<<i<<"),  buf="<<buf<<", line="<<line[1]<<std::endl;
          if(line[1][i]=='('){
            name.assign(buf);
            buf.clear();
          }
          else if(line[1][i]==',' || line[1][i]==')'){
            if (paramNum==0){//output
              paramNum++;
              output= findWireByName(buf);
              buf.clear();
            }else if (paramNum>0){//inputs
              paramNum++;
              inputs.push_back(findWireByName(buf));
              buf.clear();
            }

          }else{
            buf.push_back(line[1][i]);
          }
          i++;
        }
        modules->push_back(new And(inputs,output));
        modules->back()->name.assign(name);

      }else if (line[0].compare("not")==0){
        unsigned int i=0;
        unsigned int paramNum=0;
        std::string buf;
        std::string name;
        Wire* input;
        Wire* output;
        while(i<line[1].size()){
          //std::cout<<"here ("<<i<<"),  buf="<<buf<<", line="<<line[1]<<std::endl;
          if(line[1][i]=='('){
            name.assign(buf);
            buf.clear();
          }
          else if(line[1][i]==',' || line[1][i]==')'){
            if (paramNum==0){//output
              paramNum++;
              output=findWireByName(buf);
              buf.clear();
            }else if (paramNum>0){//input
              paramNum++;
              input=findWireByName(buf);
              buf.clear();
            }

          }else{
            buf.push_back(line[1][i]);
          }
          i++;
        }
        modules->push_back(new Not(input,output));
        modules->back()->name.assign(name);
      }else if (line[0].compare("or")==0){
        unsigned int i=0;
        unsigned int paramNum=0;
        std::string buf;
        std::string name;
        std::vector<Wire*> inputs;
        Wire* output;
        while(i<line[1].size()){
          //std::cout<<"here ("<<i<<"),  buf="<<buf<<", line="<<line[1]<<std::endl;
          if(line[1][i]=='('){
            name.assign(buf);
            buf.clear();
          }
          else if(line[1][i]==',' || line[1][i]==')'){
            if (paramNum==0){//output
              paramNum++;
              output= findWireByName(buf);
              buf.clear();
            }else if (paramNum>0){//inputs
              paramNum++;
              inputs.push_back(findWireByName(buf));
              buf.clear();
            }

          }else{
            buf.push_back(line[1][i]);
          }
          i++;
        }
        modules->push_back(new Or(inputs,output));
        modules->back()->name.assign(name);
      }else if (line[0].compare("nand")==0){
        unsigned int i=0;
        unsigned int paramNum=0;
        std::string buf;
        std::string name;
        std::vector<Wire*> inputs;
        Wire* output;
        while(i<line[1].size()){
          //std::cout<<"here ("<<i<<"),  buf="<<buf<<", line="<<line[1]<<std::endl;
          if(line[1][i]=='('){
            name.assign(buf);
            buf.clear();
          }
          else if(line[1][i]==',' || line[1][i]==')'){
            if (paramNum==0){//output
              paramNum++;
              output= findWireByName(buf);
              buf.clear();
            }else if (paramNum>0){//inputs
              paramNum++;
              inputs.push_back(findWireByName(buf));
              buf.clear();
            }

          }else{
            buf.push_back(line[1][i]);
          }
          i++;
        }
        modules->push_back(new Nand(inputs,output));
        modules->back()->name.assign(name);
      }else if (line[0].compare("nor")==0){
        unsigned int i=0;
        unsigned int paramNum=0;
        std::string buf;
        std::string name;
        std::vector<Wire*> inputs;
        Wire* output;
        while(i<line[1].size()){
          //std::cout<<"here ("<<i<<"),  buf="<<buf<<", line="<<line[1]<<std::endl;
          if(line[1][i]=='('){
            name.assign(buf);
            buf.clear();
          }
          else if(line[1][i]==',' || line[1][i]==')'){
            if (paramNum==0){//output
              paramNum++;
              output= findWireByName(buf);
              buf.clear();
            }else if (paramNum>0){//inputs
              paramNum++;
              inputs.push_back(findWireByName(buf));
              buf.clear();
            }

          }else{
            buf.push_back(line[1][i]);
          }
          i++;
        }
        modules->push_back(new Nor(inputs,output));
        modules->back()->name.assign(name);
      }else if (line[0].compare("endmodule")==0){
        state=DONE;
      }
    }
  }
}


Wire* Processor::findWireByName(std::string& name){
  for(Wire * w: (*wires_others)){
    if(name.compare(w->name)==0){
      return w;
    }
  }
  for(Wire * w: (*wires_outputs)){
    if(name.compare(w->name)==0){
      return w;
    }
  }
  for(Wire * w: (*wires_inputs)){
    if(name.compare(w->name)==0){
      return w;
    }
  }
  for(Wire * w: (*wires_ff)){
    if(name.compare(w->name)==0){
      return w;
    }
  }
  return nullptr;
}
