
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

void read_in_file();
void getEdges();

std::vector<std::vector<unsigned long long> > src_file;
std::string path;
std::string output_path;
struct Edge{
  unsigned long long start;
  unsigned long long end;
  unsigned long long count;
  unsigned long long input;
  double freq;
  double input_freq;
};
std::vector<Edge*> edges;

int main(int argc, char* argv[]){
  path="../complete/";
  path.append(argv[1]);
  read_in_file();
  // for(std::vector<unsigned long long > v : src_file){
  //   for(unsigned long long u: v){
  //     std::cout<<u<<" ";
  //   }
  //   std::cout<<""<<std::endl;
  // }
  getEdges();


}


void getEdges(){
  std::vector< std::vector<int> > inserted;
  for(unsigned long long i=0;i<src_file.size();i++){
    inserted.push_back(std::vector<int>(src_file.size()));
    for(unsigned long long h = 0;h<inserted.back().size();h++){
      inserted.back()[h]=-1;
    }
  }

  for(unsigned long long j=0;j<src_file.size();j++){
    for(unsigned long long k=0;k<src_file[j].size();k++){
      if(j==0){
        //do nothing because is initial state
      }
      else if(inserted[j][src_file[j][k]]==-1){
        Edge* e = new Edge();
        e->start=j;
        e->end=src_file[j][k];
        e->count=1;
        e->input_freq=input_freqs[k];
        inserted[e->start][e->end]=edges.size();
        //inserted[src_file[j][k]][j]=edges.size();
        edges.push_back(e);
      }else{
        edges[inserted[j][src_file[j][k]]]->count++;
        edges[inserted[j][src_file[j][k]]]->input_freq+=input_freqs[k];
      }
    }
  }


  for(Edge* e : edges){
    std::cout<<"{"<<e->start<<","<<e->end<<"} Count:"<<e->count<<" freq:"<<e->input_freq<<std::endl;
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
