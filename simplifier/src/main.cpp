


#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>
#include <algorithm>




void read_in_file();
void eliminate_dup_cols();
void eliminate_dup_rows();
void del_unreachable_states();
void write_file();
void decrease_all_above(unsigned long long i);



std::string path;
std::string output_path;
std::vector<std::vector<unsigned long long> > src_file;

int main(int argc, char* argv[]){
  path=argv[1];
  output_path.assign(path);
  output_path.pop_back();
  output_path.pop_back();
  output_path.pop_back();
  output_path.pop_back();
  output_path.append("_simp.csv");
  read_in_file();
  std::cout<<"deleting unreachable states..."<<std::endl;
  del_unreachable_states();

  std::cout<<"eliminating duplicate columns..."<<std::endl;
  eliminate_dup_cols();





  write_file();
  std::cout<<output_path<<" outputed"<<std::endl;

}


void eliminate_dup_rows(){
  
}



void decrease_all_above(unsigned long long i){
  for(size_t i=0;i<src_file.size();i++){
    for(size_t j=0;j<src_file[j].size();j++){
      if(src_file[i][j]>i){
        src_file[i][j]--;
      }
    }
  }

}
void write_file(){
  std::ofstream FILE;
  FILE.open(output_path);
  for (size_t i = 0; i < src_file.size(); i++) {
    for (size_t j = 0; j < src_file[i].size(); j++) {
      FILE<<src_file[i][j];
      if(j!=(src_file[i].size()-1)){
        FILE<<",";
      }else{
        FILE<<"\n";
      }
    }
  }

  FILE.close();
}


//also needs to do bookeeping for re-assigning state names
void del_unreachable_states(){
  bool first_deleted=false;
  std::unordered_set< unsigned long long> reachable;
  for(std::vector<unsigned long long> v : src_file ){
    for(unsigned long long ull: v){
      reachable.insert(ull);
    }
  }

  unsigned long long i=0;
  while(i<src_file.size()){
    if(!reachable.count(i)){
      if(!first_deleted){
        first_deleted=true;
      }else{
        src_file.erase(src_file.begin()+i);
        decrease_all_above(i);
      }

    }
    i++;
  }



}

void eliminate_dup_cols(){
  for(size_t i=0;i<src_file[0].size()-1;i++){//i is the column index
    for(size_t j=i+1;j<src_file[0].size();j++){//j is index of all cols to the right
      bool same=true;
      for(size_t k=0;k<src_file.size() && same;k++){//k is the row index for each col to compare
        if(src_file[k][i] != src_file[k][j]){
          same=false;
        }

      }

      if(same){
        for(size_t l=0;l<src_file.size();l++){
          src_file[l].erase(src_file[l].begin()+j);
        }
        j--;
      }
    }
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
