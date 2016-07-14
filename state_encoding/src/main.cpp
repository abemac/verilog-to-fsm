
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

void read_in_files();
void getEdges();

std::vector<std::vector<unsigned long long> > src_file;
std::string path;
std::string input_freqs_path;
std::string output_path;
struct Edge{
  unsigned long long start;
  unsigned long long end;
  unsigned long long count;
  unsigned long long input;
  double freq;
};
std::vector<Edge*> edges;
std::vector<double> input_freqs;

int main(int argc, char* argv[]){
  path="../complete/";
  path.append(argv[1]);
  input_freqs_path="input_data/";
  input_freqs_path.append(argv[1]);
  input_freqs_path.append(".inputdata");
  read_in_files();
  // for(std::vector<unsigned long long > v : src_file){
  //   for(unsigned long long u: v){
  //     std::cout<<u<<" ";
  //   }
  //   std::cout<<""<<std::endl;
  // }
  getEdges();


}

///start ::  get doubles from .inputdata and incorperate into setting freqencies

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
      if(inserted[j][src_file[j][k]]==-1){
        Edge* e = new Edge();
        e->start=j;
        e->end=src_file[j][k];
        e->count=1;
        inserted[j][src_file[j][k]]=edges.size();
        //inserted[src_file[j][k]][j]=edges.size();
        edges.push_back(e);
      }else{
        edges[inserted[j][src_file[j][k]]]->count++;
      }
    }
  }
  for(Edge* e : edges){
    std::cout<<"{"<<e->start<<","<<e->end<<"} Count:"<<e->count<<std::endl;
  }

}

void read_in_files(){
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

  input_freqs=std::vector<double>(src_file[1].size());
  std::ifstream FILE2 (input_freqs_path);

  while(std::getline(FILE2,str)){
    int input=std::stoi(str.substr(0,1),nullptr,10);
    double freq=std::stod(str.substr(2,str.size()-2),nullptr);
    input_freqs[input]=freq;

  }
  FILE2.close();



}
