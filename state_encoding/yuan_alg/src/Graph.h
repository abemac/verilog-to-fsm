#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph{

public:

  Graph();

  struct Node {
    int val;
    std::vector<Node*> adj;
    Node* parent;
    bool visited;


  };
  std::vector<Node*> vertices;

  struct Path{
    std::vector<int> path;
  };

  std::vector<Path*>* elementary_paths();


  void build_test_graph();




};

#endif
