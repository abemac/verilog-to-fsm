


#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>
#include <algorithm>
#include <list>
#include <iterator>

void iterate_until_no_change();
void read_in_file();
void eliminate_dup_cols();
void eliminate_dup_rows();
void del_unreachable_states();
void write_file();
void decrease_all_above(unsigned long long k);
void replace(unsigned long long k,unsigned long long l);
void resetIds();
void applyIdsToSrcFile();
void setLL();
void LL_to_src();

std::string path;
std::string output_path;
std::vector<std::vector<unsigned long long> > src_file;
std::vector<unsigned long long> init_state;
std::vector<unsigned long long> ids;
std::list<std::vector<unsigned long long> > src_file_ll;
std::list<std::vector<unsigned long long> >::iterator itr;

int main(int argc, char* argv[]){
  path=argv[1];
  output_path.assign(path);
  output_path.pop_back();
  output_path.pop_back();
  output_path.pop_back();
  output_path.pop_back();
  output_path.append("_simp.csv");
  system("setterm -cursor off");
  std::cout<<"reading in file..."<<std::endl;
  read_in_file();
  resetIds();
  std::cout<<"deleting unreachable states..."<<std::endl;
  del_unreachable_states();

  std::cout<<"eliminating duplicate columns..."<<std::endl;
  eliminate_dup_cols();

  std::cout<<"eliminating duplicate rows..."<<std::endl;
  eliminate_dup_rows();

  std::cout<<"checking if fully simplified..."<<std::endl;
  std::cout<<""<<std::endl;
  iterate_until_no_change();
  applyIdsToSrcFile();

  write_file();
  std::cout<<output_path<<" outputed"<<std::endl;

  system("setterm -cursor on");
}

void setLL(){
  src_file_ll.clear();
  for(std::vector<unsigned long long > v : src_file){
    src_file_ll.push_back(v);
  }
}
void LL_to_src(){
  src_file.clear();
  for(std::vector<unsigned long long > v : src_file_ll){
    src_file.push_back(v);
  }
}

void resetIds(){
  ids.clear();
  ids = std::vector<unsigned long long>( src_file.size());
  for(unsigned long long i = 0;i<src_file.size();i++){
    ids[i]=i;
  }

}


void applyIdsToSrcFile(){
  for(unsigned long long i =0 ;i<src_file.size();i++){
    for(unsigned long long j=0;j<src_file[i].size();j++){
      src_file[i][j] = ids[src_file[i][j]];
    }
  }
}
void decrease_all_above(unsigned long long k){
  for(unsigned long long j=0;j<ids.size();j++){
    if(ids[j] > k){
      ids[j]--;
    }
  }

}
//replace all k with l
void replace(unsigned long long k,unsigned long long l){
  for(unsigned long long j=0;j<ids.size();j++){
    if(ids[j] == k){
      ids[j]=l;
    }
  }
}

void iterate_until_no_change(){
  bool change = false;
  //columns:
  for(unsigned long long i=0;i<src_file[0].size()-1;i++){//i is the column index
    for(unsigned long long j=i+1;j<src_file[0].size();j++){//j is index of all cols to the right
      bool same=true;
      for(unsigned long long k=0;k<src_file.size() && same;k++){//k is the row index for each col to compare
        if(ids[src_file[k][i]] != ids[src_file[k][j]]){
          same=false;
        }

      }

      if(same){
        for(unsigned long long l=0;l<src_file.size();l++){
          src_file[l].erase(src_file[l].begin()+j);
          change=true;
        }
        j--;
      }
    }
    std::cout<<"\e[Aeliminating duplicate columns...("<<i<<"/"<<src_file[0].size()-1<<")        "<<std::endl;
  }


  bool change2=false;
  for(unsigned long long i =0;i<src_file.size()-1;i++){//i is the compare to row index
    for(unsigned long long j=i+1;j<src_file.size();j++){//j is the index of comparison
      bool same=true;
      for(unsigned long long k=0;k<src_file[j].size() && same;k++){//k is the column index
        if(ids[src_file[i][k]]!=ids[src_file[j][k]]){
          same=false;
        }
      }
      if(same){
        src_file.erase(src_file.begin()+j);
        replace(j,i);
        decrease_all_above(j);
        j--;
        change2=true;
      }
    }
    std::cout<<"\e[Aeliminating duplicate rows...("<<i<<"/"<<src_file.size()-1<<")        "<<std::endl;
  }

  if(change || change2){
    iterate_until_no_change();
  }



}

void eliminate_dup_rows(){
  for(unsigned long long i =0;i<src_file.size()-1;i++){//i is the compare to row index
    for(unsigned long long j=i+1;j<src_file.size();j++){//j is the index of comparison
      bool same=true;
      for(unsigned long long k=0;k<src_file[j].size() && same;k++){//k is the column index
        if(ids[src_file[i][k]]!=ids[src_file[j][k]]){
          same=false;
        }
      }
      if(same){
        src_file.erase(src_file.begin()+j);
        replace(j,i);//combine this line and line below??***********************
        decrease_all_above(j);
        j--;
      }
    }
    std::cout<<"\e[Aeliminating duplicate rows...("<<i<<"/"<<src_file.size()-1<<")      "<<std::endl;
  }
}



void write_file(){
  std::ofstream FILE;
  FILE.open(output_path);
  for (unsigned long long i = 0; i < src_file.size(); i++) {
    for (unsigned long long j = 0; j < src_file[i].size(); j++) {
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


//does bookeeping for re-assigning state names
void del_unreachable_states(){
  unsigned long long num_deleted=0;
  std::unordered_set< unsigned long long> reachable;
  unsigned long long ic=0;
  for(std::vector<unsigned long long> v : src_file ){
    std::cout<<"\e[Adeleting unreachable states...("<<ic<<"/"<<src_file.size()<<"), (0/?)    "<<std::endl;
    for(unsigned long long ull: v){
      reachable.insert(ull);
    }
    ic++;
  }
  // std::cout<<reachable.count(1)<<std::endl;
  unsigned long long i=0;
  setLL();
  itr=src_file_ll.begin();
  while(i<src_file.size()){
    std::cout<<"\e[Adeleting unreachable states...("<<ic<<"/"<<ic<<"),("<<i<<"/"<<src_file.size()<<")    "<<std::endl;
    if(!reachable.count(i)){
      itr=src_file_ll.erase(itr);
      decrease_all_above(i-num_deleted);
      num_deleted++;
    }else{
      ++itr;
    }
    i++;

  }
  LL_to_src();


}

void eliminate_dup_cols(){
  for(unsigned long long i=0;i<src_file[0].size()-1;i++){//i is the column index
    for(unsigned long long j=i+1;j<src_file[0].size();j++){//j is index of all cols to the right
      bool same=true;
      for(unsigned long long k=0;k<src_file.size() && same;k++){//k is the row index for each col to compare
        if(ids[src_file[k][i]] != ids[src_file[k][j]]){
          same=false;
        }

      }

      if(same){
        for(unsigned long long l=0;l<src_file.size();l++){
          src_file[l].erase(src_file[l].begin()+j);
        }
        j--;
      }
    }
    std::cout<<"\e[Aeliminating duplicate columns...("<<i<<"/"<<src_file[0].size()-1<<")      "<<std::endl;
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
