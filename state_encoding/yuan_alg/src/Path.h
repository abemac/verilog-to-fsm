#ifndef PATH_H
#define PATH_H

#include <list>

class Path{
public:
  std::list<int> path;
  static bool lessThan(const Path* p1, const Path* p2);
  static bool equalTo(const Path* p1,const Path* p2);
};

#endif
