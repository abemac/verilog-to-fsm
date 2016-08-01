#include "Graph.h"
#include <fstream>
#include <iostream>
#include <list>
#include <limits>
#include <string>
#include <iterator>

int Graph::times_graph_printed=0;

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

void Graph::encode_BFS(){
  createCodeVector();
  for(CodeLevel * cl : codeLevels){
    for(Code * c : cl->codes){
      std::cout<<(c->val)<<" ";
    }
    std::cout<<std::endl;
  }

}


unsigned long long Graph::getBestNextEncoding(unsigned long long current_enc){
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

void Graph::createCodeVector(){
  //first, find how many flip flops
  numFlipFlops=0;
  unsigned long long count=1;
  while(count<vertices.size()){
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

void Graph::delete_min_edges(){
  //in elementary_paths with odd number, delete min edge
  bool deleted [vertices.size()][vertices.size()];
  for(unsigned int i =0;i<vertices.size();i++){
    for(unsigned int j =0;j<vertices.size();j++){
      deleted[i][j]=false;
    }
  }
  // weights[1][9]=0;
  // weights[1][2]=0;
  // weights[5][7]=0;
  // weights[5][6]=0;
  for(Path * p : paths){
    if(p->path.size()%2==0){//odd number of EDGES
      //delete one with min weight from graph
      bool already_deleted_from=false;
      int del_from;
      int del_to;
      int current_weight=std::numeric_limits<int>::max();
      unsigned int i =0;
      auto itr = p->path.begin();
      while(i <= p->path.size()-2 && !already_deleted_from){
        //std::cout<<*itr<<"   "<<*(std::next(itr))<<std::endl;
        if(weights[ *itr ][ *(std::next(itr)) ] < current_weight){
          del_from=*itr;
          del_to=*(std::next(itr));
          current_weight=weights[del_from][del_to];
        }
        if(deleted[ *itr ][ *(std::next(itr))  ]){
          already_deleted_from=true;
        }
        i++;
        itr++;
      }

      if(!already_deleted_from){
        //std::cout<<"here"<<std::endl;
        bool deleted2 = false;
        for(unsigned int j=0;j< (vertices[del_from]->adj).size() && !deleted2;j++){
          if(del_to ==  (vertices[del_from]->adj)[j]->val){
            //delete parent pointer too
            bool deleted3=false;
            for(unsigned int k =0;k<(vertices[del_from]->adj)[j]->par.size() && !deleted3;k++){
              if(del_from == (vertices[del_from]->adj)[j]->par[k]->val){
                (vertices[del_from]->adj)[j]->par.erase(vertices[del_from]->adj[j]->par.begin()+k);
                deleted3=true;
              }
            }

            (vertices[del_from]->adj).erase(vertices[del_from]->adj.begin()+j);
            deleted2=true;
            deleted[del_from][del_to]=true;

          }
        }
      }




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

  //*******************
  //
  // insertEdge(0,1);
  // insertEdge(1,2);
  // insertEdge(2,4);
  // insertEdge(3,5);
  // insertEdge(3,1);
  // insertEdge(4,2);
  // insertEdge(4,3);
  // insertEdge(5,6);
  // insertEdge(6,7);
  // insertEdge(7,5);
  // insertEdge(7,8);
  // insertEdge(8,9);
  // insertEdge(9,7);
  // insertEdge(9,0);


}


void Graph::write_to_dot(){
  std::ofstream FILE;
  std::string path ="graph";
  path.append(std::to_string(Graph::times_graph_printed));
  Graph::times_graph_printed++;
  path.append(".dot");
  FILE.open(path);
  FILE<<"digraph fsm {\n";
  for(Node* n : vertices){
    for(Node* adj : n->adj){
      FILE<<(n->val)<<" -> "<<adj->val<<";\n";
    }
  }
  FILE<<"}";
  FILE.close();
}


void Graph::write_to_dot_ud(){
  std::ofstream FILE;
  std::string path ="graph";
  path.append(std::to_string(Graph::times_graph_printed));
  Graph::times_graph_printed++;
  path.append(".dot");
  FILE.open(path);
  FILE<<"graph fsm {\n";
  for(Node* n : vertices){
    for(Node* adj : n->adj){
      FILE<<(n->val)<<" -- "<<adj->val<<";\n";
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
  (ne->par).push_back(ns);



}


//private
