#include <iostream>

class Vector {
  int sz;        // size
  double* elem;  // pointer to elements
 public:
  Vector(int s) : sz{s}, elem{new double[s]} { /* ... */ }

  // copy constructor
  Vector(const Vector& v) : sz{v.sz}, elem{new double[v.sz]} {
    std::copy(v.elem, v.elem + v.sz, elem);
  }

  // copy assignment operator
  Vector& operator=(const Vector& v);

  ~Vector() { delete[] elem; }

  double& operator[](int n) { return elem[n]; }
};

Vector& Vector::operator=(const Vector& v) {
  double* p = new double[v.sz];
  std::copy(v.elem, v.elem + v.sz, p);
  delete[] elem;  // free old memory
  elem = p;       // reset elem
  sz = v.sz;
  return *this;  // return a self-reference
}

int main() {
  Vector v(3);  // define a vector of 3 elements
  v[2] = 7.7;
  Vector v2 = v;
  // if copy constructor not defined:
  // compiler provides a default copy constructor
  // that performs a shallow copy of the elem pointer
  // both v and v2 point to the same memory location

  // if copy constructor is defined:
  // a deep copy is performed, and v2 has its own copy of the elements

  v2[2] = 9.9;

  std::cout << "v[2] = " << v[2] << std::endl;  // prints 7.7 instead of 9.9

  // on exit, does not cause double free runtime error when both v and v2 are destroyed

  // copy assignment test
  Vector v3(5);

  v3 = v;  // uses copy assignment operator

  std::cout << "v3[2] = " << v3[2] << std::endl;  // prints 7.7

  // try self-assignment
  v3 = v3;

  std::cout << "After self-assignment, v3[2] = " << v3[2] << std::endl;  // prints 7.7

  return 0;
}