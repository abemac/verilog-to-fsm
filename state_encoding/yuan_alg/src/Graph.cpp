#include "Graph.h"
#include <fstream>
#include <iostream>
Graph::Graph(int size){
  vertices = std::vector<Node*>(size);
  for(unsigned int i =0; i< vertices.size();i++){
    vertices[i]=nullptr;
  }

}

std::vector<Graph::Path*>* Graph::elementary_paths(){
  return nullptr;

}


void Graph::build_test_graph(){
  insertEdge(0,1);
  insertEdge(1,2);
  insertEdge(2,1);
  insertEdge(1,9);
  insertEdge(9,2);
  insertEdge(2,3);
  insertEdge(3,5);
  insertEdge(3,4);
  insertEdge(4,1);
  insertEdge(4,8);
  insertEdge(5,4);
  insertEdge(5,7);
  insertEdge(5,6);
  insertEdge(6,1);
  insertEdge(7,1);
  insertEdge(8,7);
}


void Graph::write_to_dot(){
  std::ofstream FILE;
  FILE.open("graph.dot");
  FILE<<"digraph fsm {\n";
  for(Node* n : vertices){
    for(Node* adj : n->adj){
      FILE<<(n->val)<<" -> "<<adj->val<<";\n";
    }
  }
  FILE<<"}";
  FILE.close();
}

void Graph::insertEdge(int startVal, int endVal){
  Node * ns = vertices[startVal];

  if(ns==nullptr){
    ns = new Node(startVal);
    vertices[startVal]=ns;
  }

  Node* ne = vertices[endVal];
  if(ne==nullptr){
    ne = new Node(endVal);
    vertices[endVal]= ne;
  }

  (ns->adj).push_back(ne);



}


//private
