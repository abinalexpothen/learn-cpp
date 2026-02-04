#include <iostream>

class Vector {
  int size;      // size of vector element
  double* elem;  // pointer to vector elements

 public:
  double operator[](int n) { return elem[n]; }
};

int main() {
  std::cout << "Essential operations example\n";
  return 0;
}