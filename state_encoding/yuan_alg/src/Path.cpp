#include "Path.h"
#include <algorithm>


bool Path::lessThan(const Path* p1,const Path* p2){
  return p1->path.size() < p2->path.size();
}
bool Path::equalTo(const Path* p1,const Path* p2){
  if(p1->path.size()!= p2->path.size()){
    return false;
  }
  //else, they are the same size

  std::vector<int> p3 = std::vector<int>(p1->path.begin(),p1->path.end());
  std::vector<int> p4 = std::vector<int>(p2->path.begin(),p2->path.end());

  std::sort(p3.begin(),p3.end()-1);
  std::sort(p4.begin(),p4.end()-1);

  for(unsigned int i; i< p3.size()-1;i++){
    if(p3[i]!=p4[i]){
      return false;
    }
  }

  return true;



}
