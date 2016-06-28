


#include <vector>
#include <iostream>
#include <string>
#include <fstream>



void read_in_file();

std::vector<std::vector<unsigned long long> > src_file;

int main(){
  read_in_file();
  for(std::vector<unsigned long long> v : src_file){
    for(unsigned long long ull : v){
      std::cout<<ull<<" ";
    }
    std::cout<<""<<std::endl;
  }


}


void read_in_file(){
  std::ifstream FILE("s27.csv");
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
