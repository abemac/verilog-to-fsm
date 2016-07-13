
/*

MINIMUM BIT CHANGE
•use input data to get frequency of input
•find frequency of each edge
•order states by number of unique pointers to others states
•choose state with most pointers -assign arbitrary code
•give one-off code to each of the states it points to if it doesn't already
    have a code
•repeat until all states have codes

PRIORITIZED ADJACENT
•

*/
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

void read_in_file();

std::vector<std::vector<unsigned long long> > src_file;
std::string path;
std::string output_path;

int main(int argc, char* argv[]){
  path="../complete/";
  path.append(argv[1]);
  read_in_file();
  for(std::vector<unsigned long long > v : src_file){
    for(unsigned long long u: v){
      std::cout<<u<<" ";
    }
    std::cout<<""<<std::endl;
  }

}



void read_in_file(){
  std::ifstream FILE(path);
  std::string str;

  while(std::getline(FILE,str)){
    std::vector<unsigned long long> tmp;
    unsigned long long i=0;
    unsigned long long lasti=0;
    for(;i<str.size();i++){
      if(str[i]==',' || i==str.size()-1){
        unsigned long long val=std::stoull(str.substr(lasti,i+1-lasti),nullptr,10);
        tmp.push_back(val);
        lasti=i+1;
      }
    }
    src_file.push_back(tmp);
  }

  FILE.close();


}
