#ifndef GRAPH_H
#define GRAPH_H

#include "Path.h"
#include <vector>
#include <set>

class Graph{

public:

  Graph(int size);

  struct Node {
    int val;
    std::vector<Node*> adj;
    std::vector<Node*> adj_mod;
    std::vector<Node*> par;
    std::vector<Node*> par_mod;
    Node* parent;
    bool visited;
    bool visited_2;
    bool in_path;
    unsigned long long enc1;
    unsigned long long enc2;
    Node(int val_){val=val_;}

  };



  std::vector<Path*>* elementary_paths();

  void insertEdge(int startVal,int endVal);

  void build_test_graph();
  void write_to_dot();
  void write_to_dot_ud();
  void write_to_dot_result();
  void write_to_dot_result_DFS();
  void delete_min_edges();
  void encode_BFS();
  void encode_DFS();


private:
    std::vector<Node*> vertices;
    std::vector< std::vector<int> > weights;
    std::vector<Path*> paths;
    Path* getShortestPathToSelf(Node* n);

    static int times_graph_printed;

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

    unsigned long long getBestNextEncoding(unsigned long long current_enc);
    void createCodeVector();

    std::vector<std::vector<unsigned long long> > src_file;
    std::vector<bool> usedCodes;


    std::vector< std::vector<CodeLevel*>* > codeLevels;

    unsigned long long numFlipFlops;

    //////////
    std::vector<Code*> allCodes;


};

#endif
