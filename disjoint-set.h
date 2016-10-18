#ifndef DISJOINT_SET
#define DISJOINT_SET

#include <set>
// disjoint-set forests using union-by-rank and path compression (sort of).

typedef struct {
  int rank;
  int p;
  int size;
  std::set<int> cset;
  float x;
  float y;
} uni_elt;

class universe {
public:
  universe(int elements, int width, int height);
  ~universe();
  int find(int x);  
  void join(int x, int y);
  int size(int x) const { return elts[x].size; }
  int num_sets() const { return num; }
  uni_elt* findelt(int x) {return &elts[find(x)]; }
  std::set<int> kset;

private:
  uni_elt *elts;
  int num;
  int width;
  int height;
};

universe::universe(int elements, int width, int height) {
  this->width = width;
  this->height = height;
  elts = new uni_elt[elements];
  num = elements;
  for (int i = 0; i < elements; i++) {
    elts[i].rank = 0;
    elts[i].size = 1;
    elts[i].p = i;
    elts[i].cset.insert(i);
    elts[i].y = ((i / this->width) + 1) * 1.0 / this->height;
    elts[i].x = (i - (i / this->width) * this->width + 1) * 1.0 / this->width;
    kset.insert(i);
  }
}
  
universe::~universe() 
{
  delete [] elts;
}

int universe::find(int x) 
{
  int y = x;
  while (y != elts[y].p)
    y = elts[y].p;
  elts[x].p = y;
  return y;
}

void universe::join(int x, int y) 
{
  if (elts[x].rank > elts[y].rank) { //merge y to x
    elts[x].x = (elts[x].x * elts[x].size + elts[y].x * elts[y].size) / (elts[x].size + elts[y].size);
    elts[x].y = (elts[x].y * elts[x].size + elts[y].y * elts[y].size) / (elts[x].size + elts[y].size);
    elts[y].p = x;
    elts[x].size += elts[y].size;
    elts[x].cset.insert(elts[y].cset.begin(), elts[y].cset.end());
    elts[y].cset.clear();
    kset.erase(y);
  } else { // merge x to y
    elts[y].x = (elts[x].x * elts[x].size + elts[y].x * elts[y].size) / (elts[x].size + elts[y].size);
    elts[y].y = (elts[x].y * elts[x].size + elts[y].y * elts[y].size) / (elts[x].size + elts[y].size);
    elts[x].p = y;
    elts[y].size += elts[x].size;
    elts[y].cset.insert(elts[x].cset.begin(), elts[x].cset.end());
    elts[x].cset.clear();
    if (elts[x].rank == elts[y].rank)
      elts[y].rank++;
    kset.erase(x);
  }
  num--;
}

#endif
