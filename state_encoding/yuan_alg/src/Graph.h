#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph{

public:

  struct Node {
    int val;
    std::vector<Node*> adj;

    
  };
  std::vector<Node*> vertices;

  struct Path{
    std::vector<int> path;
  };

  std::vector<Path* > * elementary_paths();



};

#endif
