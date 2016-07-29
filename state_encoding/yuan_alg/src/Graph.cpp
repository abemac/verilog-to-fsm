#include "Graph.h"
#include <fstream>
#include <iostream>
#include <list>
Graph::Graph(int size){
  vertices = std::vector<Node*>(size);
  for(unsigned int i =0; i< vertices.size();i++){
    vertices[i]=nullptr;
  }

  weights = std::vector< std::vector<int> >(size);
  for(unsigned int i=0;i<weights.size();i++){
    weights[i] = std::vector<int>(size);
    for(unsigned int j =0;j<weights[i].size();j++){
      weights[i][j] =1; //FOR NOW
    }
  }

}

Graph::Graph(const Graph& other){
  vertices=std::vector<Node*> (other.vertices);
  weights = std::vector< std::vector<int> >(vertices.size());
  for(unsigned int i=0;i<weights.size();i++){
    weights[i] = std::vector<int>(vertices.size());
    for(unsigned int j =0;j<weights[i].size();j++){
      weights[i][j] =other.weights[i][j];
    }
  }
  paths = std::vector<Path*>(other.paths);
}

void Graph::delete_min_edges(){
  //in elementary_paths with odd number, delete min edge
  bool deleted [vertices.size()][vertices.size()];
  for(Path * p : paths){
    if(p->path.size()%2==0){//odd number of EDGES
      //delete one with min weight from graph
      bool already_deleted_from=false;
      int del_from;
      int del_to;

      //start here monday -- delete edges, not worry about disconnections




    }
  }


}

std::vector<Path*>* Graph::elementary_paths(){

  for(Node* node : vertices){
    node->visited=false;
    node->in_path=false;
  }

  std::list<Node*> queue;
  queue.push_back(vertices[0]);
  queue.front()->visited=true;
  //std::vector<Path*>* paths= new std::vector<Path*>();
  while(!queue.empty()){
    Node * n = queue.front();
    queue.pop_front();
    //if(!(n->in_path)){
      Path* p = getShortestPathToSelf(n);
      if(p!=nullptr){
        bool insert=true;
        for(Path* po : paths){
          if(Path::equalTo(po,p)){
            insert=false;
          }
        }
        if(insert){
          paths.push_back(p);
        }
      }
    //}
    for(Node * adj : n->adj){
      if(adj->visited==false){
        adj->visited=true;
        queue.push_back(adj);
      }
    }
  }

  return &paths;

}


Path * Graph::getShortestPathToSelf(Node* n_){
  for(Node * node : vertices){
    node->visited_2=false;
  }
  std::list<Node*> queue;

  //skip over starting node
  for(Node * adj : n_->adj){
      adj->parent = n_;
      adj->visited_2=true;
      queue.push_back(adj);
  }

  while(!queue.empty()){
    Node* n = queue.front();
    queue.pop_front();

    if(n->val == n_->val){//found the shortest cycle
      Path* p = new Path();
      (p->path).push_front(n->val);
      Node * n2 = n->parent;
      while(n2->val != n->val){
        (p->path).push_front(n2->val);
        n2->in_path=true;
        n2=n2->parent;
      }
      (p->path).push_front(n2->val);
      n2->in_path=true;
      return p;

    }
    for(Node * adj : n->adj){
      if(adj->visited_2==false){
        adj->visited_2=true;
        adj->parent=n;
        queue.push_back(adj);
      }
    }
  }
  return nullptr;

}


void Graph::build_test_graph(){
  // insertEdge(0,1);
  // insertEdge(1,2);
  // insertEdge(2,1);
  // insertEdge(1,9);
  // insertEdge(9,2);
  // insertEdge(2,3);
  // insertEdge(3,5);
  // insertEdge(3,4);
  // insertEdge(4,1);
  // insertEdge(4,8);
  // insertEdge(5,4);
  // insertEdge(5,7);
  // insertEdge(5,6);
  // insertEdge(6,1);
  // insertEdge(7,1);
  // insertEdge(8,7);

  //*******************

  insertEdge(0,1);
  insertEdge(1,2);
  insertEdge(2,4);
  insertEdge(3,5);
  insertEdge(3,1);
  insertEdge(4,2);
  insertEdge(4,3);
  insertEdge(5,6);
  insertEdge(6,7);
  insertEdge(7,5);
  insertEdge(7,8);
  insertEdge(8,9);
  insertEdge(9,7);
  insertEdge(9,0);


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
