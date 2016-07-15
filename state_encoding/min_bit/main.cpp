
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <list>

void read_in_file();
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
}
struct CodeLevel{
  std::vector<Code*> codes;
  
}

std::vector<Node*> vertices;



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
  buildGraph();

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

  while(queue.size()!=0){
    Node * n = queue.front();

    for(Node* n2 : n->adj){
      if(n2->visited==false){
        n2->encoding = getBestNextEncoding(n2->encoding);
        n2->visited =true;
        queue.push_back(n2);
      }
    }
    queue.pop_front();
  }


  //no need to repeat for univisted vertices, because independent portions
  //of the graph will never be reached by the initial state

}


unsigned long long getBestNextEncoding(unsigned long long current_enc){

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
