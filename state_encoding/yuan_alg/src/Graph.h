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
    Node* parent;
    bool visited;
    bool visited_2;
    bool in_path;
    Node(int val_){val=val_;}


  };



  std::vector<Path*>* elementary_paths();

  void insertEdge(int startVal,int endVal);

  void build_test_graph();
  void write_to_dot();

private:
    std::vector<Node*> vertices;
    Path* getShortestPathToSelf(Node* n);


};

#endif
