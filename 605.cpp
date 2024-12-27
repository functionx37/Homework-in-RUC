#include <iostream>
using namespace std;

class Child {
public:
Child(int i){
    id = i;
  }
  int getId(){
    return id;
  }

private:
  int id;
};

class Circle {
public:
void in(Child *p){
    children[p->getId()] = p;
    capacity=p->getId();
    current = capacity;
  }
  Child* next(){
    do{
        current++;
        if(current > capacity) current = 1;
    }while(children[current]==NULL);
    return children[current];
  }
  void out(Child *p){
    children[p->getId()] = NULL;
  }

private:
  int capacity;
  int current;
  Child *children[1001];
};

int main() {
  int n, m, i, j;
  Circle circle;
  Child *child;

  cin >> n >> m;

  for (i = 1; i <= n; i++) {
    child = new Child(i);
    circle.in(child);
  }

  for (i = 1; i < n; i++) {
    for (j = 1; j <= m; j++) {
      child = circle.next();
    }
    circle.out(child);
  }

  child = circle.next();
  cout << child->getId();
  return 0;
}