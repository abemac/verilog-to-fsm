
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <list>
#include <bitset>
#include <algorithm>


void read_in_file();
void write_file();
void buildGraph();
void BFS();
unsigned long long getBestNextEncoding(unsigned long long current_enc);
void createCodeVector();

std::vector<std::vector<unsigned long long> > src_file;
std::string path;
std::string output_path;
std::vector<bool> usedCodes;

struct Node{
  unsigned long long val;
  std::vector<Node*> adj;
  bool visited;
  Node(unsigned long long i){val=i;}
  unsigned long long encoding;
};
Node* getNode(unsigned long long i);

struct Code{
  unsigned long long val;
  bool used;
  Code(unsigned long long v){
    val=v;
    used=false;
  }
};
struct CodeLevel{
  std::vector<Code*> codes;
};

Code * findCode(unsigned long long val);

std::vector<CodeLevel*> codeLevels;
std::vector<Node*> vertices;

unsigned long long numFlipFlops;

bool compareByState(const Node* a, const Node* b){
  return a->val < b->val;
}


int main(int argc, char* argv[]){
  path="../complete/";
  path.append(argv[1]);
  output_path="sa/state_assignments(";
  output_path.append(argv[1]);
  output_path.append(").txt");
  read_in_file();
  // for(std::vector<unsigned long long > v : src_file){
  //   for(unsigned long long u: v){
  //     std::cout<<u<<" ";
  //   }
  //   std::cout<<""<<std::endl;
  // }
  buildGraph();
  createCodeVector();
  BFS();

  std::sort(vertices.begin(),vertices.end(),compareByState);
  write_file();
  std::cout<<output_path<<" outputted"<<std::endl;

}
void write_file(){
  std::ofstream FILE;
  FILE.open(output_path);
  for(Node* n : vertices){
    FILE<<n->val<<":";
    if(n->val != 0 && n->encoding==0){
      FILE<<"not reachable from initial state - no encoding given";
    }
    else{
      for(int i = numFlipFlops-1; i>=0;i--){
        unsigned int x = ((n->encoding) & (1<<i))>>i;
        FILE<<x;
      }
    }
  FILE<<"\n";
  }

  FILE.close();

}



void createCodeVector(){
  //first, find how many flip flops
  numFlipFlops=0;
  unsigned long long count=1;
  while(count<src_file.size()){
    numFlipFlops++;
    count<<=1;//multiply by two
  }

  usedCodes=std::vector<bool>(count);

  //second, create Code vector.
  CodeLevel* cl1=new CodeLevel();
  cl1->codes.push_back(new Code(0));
  usedCodes[0]=true;
  codeLevels.push_back(cl1);

  unsigned long long workingVal=0;
  unsigned long long baseVal=0;
  while(codeLevels.size()<numFlipFlops+1){
    CodeLevel * prev = codeLevels.back();
    CodeLevel * next = new CodeLevel();
    for(Code * c : prev->codes){
      baseVal=c->val;
      for(unsigned long long i=0;i<numFlipFlops;i++){
        workingVal=baseVal | (1<<i);
        if(!usedCodes[workingVal]){
          next->codes.push_back(new Code(workingVal));
          usedCodes[workingVal]=true;
        }
      }
    }

    codeLevels.push_back(next);

  }

  // for(CodeLevel * cl : codeLevels){
  //   for(Code * c : cl->codes){
  //     std::cout<<(c->val)<<" ";
  //   }
  //   std::cout<<std::endl;
  // }



}

void BFS(){
  for(Node * v : vertices){
    v->visited=false;
  }
  std::list<Node*> queue;

  queue.push_back(vertices[0]);
  vertices[0]->visited=true;
  vertices[0]->encoding=0;
  vertices[0]->visited=true;
  findCode(0)->used=true;

  while(queue.size()!=0){
    Node * n = queue.front();

    for(Node* n2 : n->adj){
      if(n2->visited==false){
        n2->encoding = getBestNextEncoding(n->encoding);
        n2->visited =true;
        queue.push_back(n2);
      }
    }
    queue.pop_front();
  }


  //no need to repeat for univisted vertices, because independent portions
  //of the graph will never be reached by the initial state

}


Code * findCode(unsigned long long val_){
  for(CodeLevel* cl : codeLevels){
    for(Code* c : cl->codes){
      if(c->val == val_){
        return c;
      }
    }
  }
  return nullptr;
}

unsigned long long getBestNextEncoding(unsigned long long current_enc){
  Code* current=nullptr;
  unsigned long long loc;
  for(unsigned long long j=0;j<codeLevels.size() && current==nullptr;j++){
    for(Code* c : codeLevels[j]->codes){
      if(c->val == current_enc){
        current=c;
        loc=j;
      }
    }
  }

  unsigned long long distanceAway=1;
  while(distanceAway <= numFlipFlops){
    if(distanceAway==2){//try in same column first
      for(Code * cc : codeLevels[loc]->codes){
        if(cc->used == false){
          cc->used=true;
          return cc->val;
        }
      }
    }
    //otherwise, try to the right and left
    if(loc<codeLevels.size()-distanceAway){
      for(Code * c : codeLevels[loc+distanceAway]->codes){
        if(c->used==false){
          c->used=true;
          return c->val;
        }
      }
    }
    if(loc>distanceAway){
      for(Code * c : codeLevels[loc-distanceAway]->codes){
        if(c->used==false){
          c->used=true;
          return c->val;
        }
      }
    }

    distanceAway++;

  }
  return -1;
}

Node* getNode(unsigned long long i){
  for( Node* n : vertices){
    if(n->val == i){
      return n;
    }
  }
  //otherwise, create node
  Node * n2=new Node(i);
  vertices.push_back(n2);
  return n2;
}


void buildGraph(){
  //first, cut down duplicate edges in matrix
  for(unsigned long long i=0;i<src_file.size();i++){
    std::set<unsigned long long> s( src_file[i].begin(), src_file[i].end() );
    src_file[i].assign( s.begin(), s.end() );
  }
  //next, create a vertex for each state and add its edges to its
  //adjacent list

  for(unsigned long long i=0;i<src_file.size();i++){
    Node * n = getNode(i);
    for(unsigned long long u : src_file[i]){
      (n->adj).push_back(getNode(u));
    }


  }
  // for(Node * n : vertices){
  //   std::cout<<(n->val)<<": ";
  //    for(Node * n2 : n->adj){
  //      std::cout<<(n2->val)<<" ";
  //    }
  //   std::cout<<std::endl;
  // }



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
