#include "Graph.h"
#include <fstream>
#include <iostream>
#include <list>
#include <limits>
#include <string>
#include <iterator>
#include <bitset>

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

void Graph::choose_best_encoding(){
  int num_bit_flips_BFS=0;
  int num_bit_flips_DFS=0;
  int num_bit_flips_BFS_weighted=0;
  int num_bit_flips_DFS_weighted=0;


  for(Node *n : vertices){
    unsigned long long enc1=n->enc1;
    unsigned long long enc2=n->enc2;
    for(Node * a: n->adj){
      int dif1=0;
      int dif2=0;
      unsigned int i =0;
      unsigned long long enc12=a->enc1;
      unsigned long long enc22=a->enc2;
      while(i<numFlipFlops){
        if(  ((enc1>>i)&1) != ((enc12>>i)&1)  ){
          dif1++;
        }
        if(  ((enc2>>i)&1) != ((enc22>>i)&1)  ){
          dif2++;
        }
        i++;
      }
      //std::cout<<dif1<<" "<<dif2<<std::endl;
      num_bit_flips_BFS+=dif1;
      num_bit_flips_DFS+=dif2;
      num_bit_flips_BFS_weighted+=dif1*weights[n->val][a->val];
      num_bit_flips_DFS_weighted+=dif2*weights[n->val][a->val];



    }

  }
  std::cout<<"Bit flips BFS: Unweighted: "<<num_bit_flips_BFS<<" Weighted: "<<num_bit_flips_BFS_weighted<<std::endl;
  std::cout<<"Bit flips DFS: Unweighted: "<<num_bit_flips_DFS<<" Weighted: "<<num_bit_flips_DFS_weighted<<std::endl;
}

void Graph::encode_DFS(){
  //weights[3][5]=9000;
  for(Code* c : allCodes){
    c->used=false;
  }

  for(Node * v : vertices){
    v->visited=false;
  }
  std::list<Node*> stack;

  int sum=0;
  int loc=0;

  for(unsigned int i =0;i<weights.size();i++){
    int tmp_sum=0;
    for(int j : weights[i]){
      tmp_sum+=j;
    }
    if(tmp_sum > sum){
      sum=tmp_sum;
      loc=i;
    }
  }
  stack.push_back(vertices[loc]);
  vertices[loc]->visited=true;
  vertices[loc]->enc2=0;
  vertices[loc]->visited=true;
  allCodes[loc]->used=true;

  while(stack.size()!=0){
    Node * n = stack.back();
    stack.pop_back();

    for(Node* n2 : n->adj_mod){
      if(n2->visited==false){
        n2->enc2 = getBestNextEncoding(n->enc2);
        n2->visited =true;
        stack.push_back(n2);
      }
    }
    for(Node* n3 : n->par_mod){
      if(n3->visited==false){
        n3->enc2 = getBestNextEncoding(n->enc2);
        n3->visited =true;
        stack.push_back(n3);
      }
    }


  }


}

void Graph::encode_BFS(){
  createCodeVector();
  for(Node * v : vertices){
    v->visited=false;
  }
  std::list<Node*> queue;

  //choose node with max weight
  //whichever row in weights has max sum is vertix with most weight

  int sum=0;
  int loc=0;

  for(unsigned int i =0;i<weights.size();i++){
    int tmp_sum=0;
    for(int j : weights[i]){
      tmp_sum+=j;
    }
    if(tmp_sum > sum){
      sum=tmp_sum;
      loc=i;
    }
  }



  queue.push_back(vertices[loc]);
  vertices[loc]->visited=true;
  vertices[loc]->enc1=0;
  vertices[loc]->visited=true;
  allCodes[loc]->used=true;

  while(queue.size()!=0){
    Node * n = queue.front();

    for(Node* n2 : n->adj_mod){
      if(n2->visited==false){
        n2->enc1 = getBestNextEncoding(n->enc1);
        n2->visited =true;
        queue.push_back(n2);
      }
    }
    for(Node* n3 : n->par_mod){
      if(n3->visited==false){
        n3->enc1 = getBestNextEncoding(n->enc1);
        n3->visited =true;
        queue.push_back(n3);
      }
    }

    queue.pop_front();
  }

}

unsigned long long Graph::getBestNextEncoding(unsigned long long current_enc){
  unsigned long long distanceAway=1;
  while(distanceAway <= numFlipFlops){
    for(Code * c : codeLevels[current_enc]->operator[](distanceAway)->codes){
      if(c->used==false){
        c->used=true;
        return c->val;
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
  for(unsigned long long j=0;j<usedCodes.size();j++){
    usedCodes[j]=false;
  }
  allCodes=std::vector<Code*>(count);
  for(unsigned long long i =0; i<count;i++){
    allCodes[i]=new Code(i);
  }
  codeLevels=std::vector< std::vector<CodeLevel*>* >();

  for(unsigned long long num=0;num<count;num++){
    for(unsigned long long j=0;j<usedCodes.size();j++){
      usedCodes[j]=false;
    }
    std::vector<CodeLevel*>* codeLevel_current= new std::vector<CodeLevel*>;
    CodeLevel* cl1=new CodeLevel();
    cl1->codes.push_back(allCodes[num]);
    usedCodes[num]=true;
    codeLevel_current->push_back(cl1);

    unsigned long long workingVal=0;
    unsigned long long baseVal=0;
    while(codeLevel_current->size()<numFlipFlops+1){
      CodeLevel * prev = codeLevel_current->back();
      CodeLevel * next = new CodeLevel();
      for(Code * c : prev->codes){
        baseVal=c->val;
        for(unsigned long long i=0;i<numFlipFlops;i++){
          workingVal=baseVal ^ (1<<i);
          if(!usedCodes[workingVal]){
            next->codes.push_back(allCodes[workingVal]);
            usedCodes[workingVal]=true;
          }
        }
      }

      codeLevel_current->push_back(next);

    }
    codeLevels.push_back(codeLevel_current);
  }


  // for(std::vector<CodeLevel*> * v : codeLevels){
  //   for(CodeLevel* cl: *v ){
  //     for(Code * c : cl->codes){
  //       std::bitset<4> b (c->val);
  //       std::cout<<b<<" ";
  //     }
  //     std::cout<<std::endl;
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
        for(unsigned int j=0;j< (vertices[del_from]->adj_mod).size() && !deleted2;j++){
          if(del_to ==  (vertices[del_from]->adj_mod)[j]->val){
            //delete parent pointer too
            bool deleted3=false;
            for(unsigned int k =0;k<(vertices[del_from]->adj_mod)[j]->par_mod.size() && !deleted3;k++){
              if(del_from == (vertices[del_from]->adj_mod)[j]->par_mod[k]->val){
                (vertices[del_from]->adj_mod)[j]->par_mod.erase(vertices[del_from]->adj_mod[j]->par_mod.begin()+k);
                deleted3=true;
              }
            }

            (vertices[del_from]->adj_mod).erase(vertices[del_from]->adj_mod.begin()+j);
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
    for(Node* adj_mod : n->adj_mod){
      FILE<<(n->val)<<" -- "<<adj_mod->val<<";\n";
    }
  }
  FILE<<"}";
  FILE.close();



}

void Graph::write_to_dot_result_BFS(){
  std::ofstream FILE;
  std::string path ="BFS.dot";
  FILE.open(path);
  FILE<<"digraph fsm {\n";
  for(Node* n : vertices){
    for(Node* adj : n->adj){
      FILE<<"\""<<(n->val)<<"\\n(";
      for(int i = numFlipFlops-1; i>=0;i--){
        unsigned int x = ((n->enc1) & (1<<i))>>i;
        FILE<<x;
      }
      FILE<<")\" -> \""<<adj->val<<"\\n(";
      for(int i = numFlipFlops-1; i>=0;i--){
        unsigned int x = ((adj->enc1) & (1<<i))>>i;
        FILE<<x;
      }


      FILE<<")\";\n";
    }
  }
  FILE<<"}";
  FILE.close();

}

void Graph::write_to_dot_result_DFS(){
  std::ofstream FILE;
  std::string path ="DFS.dot";
  FILE.open(path);
  FILE<<"digraph fsm {\n";
  for(Node* n : vertices){
    for(Node* adj : n->adj){
      FILE<<"\""<<(n->val)<<"\\n(";
      for(int i = numFlipFlops-1; i>=0;i--){
        unsigned int x = ((n->enc2) & (1<<i))>>i;
        FILE<<x;
      }
      FILE<<")\" -> \""<<adj->val<<"\\n(";
      for(int i = numFlipFlops-1; i>=0;i--){
        unsigned int x = ((adj->enc2) & (1<<i))>>i;
        FILE<<x;
      }


      FILE<<")\";\n";
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
  (ns->adj_mod).push_back(ne);
  (ne->par).push_back(ns);
  (ne->par_mod).push_back(ns);


}


//private
