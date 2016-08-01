


#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "Graph.h"
#include "Path.h"
int main(){
  Graph g = Graph(10);
  g.build_test_graph();
  g.write_to_dot();
  std::vector<Path*>* paths= g.elementary_paths();
  std::sort(paths->begin(),paths->end(),Path::lessThan);
  for(Path *p : *paths){
    std::cout<<"Path: {";
    for(int i : p->path){
      std::cout<<i<<",";
    }
    std::cout<<"\b}"<<std::endl;
  }

  Graph g2= Graph(g);
  //g2.write_to_dot();
  g2.delete_min_edges();
  g2.write_to_dot_ud();





}
