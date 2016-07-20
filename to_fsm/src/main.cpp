#include <vector>
#include "Wire.h"
#include "Module.h"
#include "modules/And.h"
#include "modules/Not.h"
#include "modules/Or.h"
#include "modules/Nand.h"
#include "modules/Nor.h"
#include "modules/Dff.h"
#include "Processor.h"
#include "Simulator.h"
#include <iostream>
#include <fstream>
#include <string>

std::vector<Wire*> wires_inputs; //enter wires in order encountered
std::vector<Wire*> wires_ff;//flip_flop outputs
std::vector<Wire*> wires_outputs;
std::vector<Wire*> wires_others;

std::vector<Module*> modules;
std::vector<Module*> modules_ff;

std::vector<std::vector<std::string> > src_file;

std::string path;
std::string output_path;

void read_in_file();

int main(int argc, char* argv[]){
  path=argv[1];
  output_path.assign(path.substr(8,path.size()-8));
  output_path[output_path.size()-1]='c';
  output_path.push_back('s');
  output_path.push_back('v');
  system("setterm -cursor off");
  std::cout<<"\n**Reading in circuit from file..."<<std::endl;
  read_in_file();
  std::cout<<"Done Reading in file."<<std::endl;
  std::cout<<"Generating data structures for Simulation... (0\% done)"<<std::endl;
  Processor p = Processor(&wires_inputs,&wires_ff,&wires_outputs,&wires_others,&modules,&modules_ff);
  p.processFile(src_file);
  std::cout<<"\e[AGenerating data structures for Simulation... (100\% done)"<<std::endl;
  std::cout<<"\n";
  for(int i=0;i<80;i++){
    std::cout<<"-";
  }
  std::cout<<""<<std::endl;
  std::cout<<"**Detected Information:"<<std::endl;
  std::cout<<"\nInputs: {";
  for(Wire* w: wires_inputs){
    std::cout<<w->name<<",";
  }
  std::cout<<"\b}"<<std::endl;

  std::cout<<"\nOutputs: {";
  for(Wire* w: wires_outputs){
    std::cout<<w->name<<",";
  }
  std::cout<<"\b}"<<std::endl;

  std::cout<<"\nOthers: {";
  for(Wire* w: wires_others){
    std::cout<<w->name<<",";
  }
  std::cout<<"\b}"<<std::endl;

  std::cout<<"\nModules: "<<std::endl;
  for(Module* m: modules){
    std::cout<<(m->name)<<" (inputs={";
    for(Wire * w : (m->inputs)){
      std::cout<<(w->name)<<",";
    }
    std::cout<<"\b},output="<<(m->output->name)<<")"<<std::endl;
  }
  for(int i=0;i<80;i++){
    std::cout<<"-";
  }
  std::cout<<"\n"<<std::endl;
  for(int i=0;i<80;i++){
    std::cout<<"-";
  }std::cout<<""<<std::endl;
  std::cout<<"**Now simulating circuit... "<<std::endl;
  Simulator s = Simulator(&wires_inputs,&wires_ff,&wires_outputs,&wires_others,&modules,&modules_ff);
  s.simulate(output_path);
  for(int i=0;i<80;i++){
    std::cout<<"-";
  }std::cout<<"\n"<<std::endl;

  system("setterm -cursor on");

}


void read_in_file(){
  std::ifstream FILE(path);
  std::string str;

  while(std::getline(FILE,str)){
    std::vector<std::string> tmp;
    unsigned int i=0;
    unsigned int lasti=0;
    while(str[i]==' ' || str[i]=='\t' || str[i]=='\n'){i++;}
    lasti=i;
    for(;i<str.size();i++){
      if(str[i]==' ' || i==str.size()-1){
        tmp.push_back(str.substr(lasti,i+1-lasti));
        tmp.back().erase(tmp.back().find_last_not_of(" \n\r\t")+1);
        lasti=i+1;
      }
    }
    src_file.push_back(tmp);
  }

  FILE.close();


}
