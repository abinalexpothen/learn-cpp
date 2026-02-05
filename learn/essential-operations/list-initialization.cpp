#include <iostream>

class Vector {
  long int sz;   // size
  double* elem;  // a pointer to the elements

 public:
  Vector(int s) : sz{s}, elem{new double[s]} {
    for (int i = 0; i < sz; i++) {
      elem[i] = 0;
    }
  }

  Vector(std::initializer_list<double> lst)
      : sz{lst.end() - lst.begin()},
        elem{new double[sz]} {  // Member initializers must appear in
                                // the order of the members themselves
                                // This means we can use a member as
                                // part of subsequent member init (like sz)
    std::copy(lst.begin(), lst.end(), elem);
  }

  double& operator[](int n) { return elem[n]; }

  ~Vector() { delete[] elem; }
};

int main() {
  /* We want to initialize it with a list of values

  Like Vector v = {1.1, 2.2, 3.3};

  How do we write a constructor that accepts initializer list?

  {T, T, T, ...} --> object of type std::initializer_list<T>

  First element: identified by begin()
  Last element: idenftified by end()
  */

  Vector v = {1.1, 2.2, 3.3, 4.4, 5.5};

  // note: initializer list is passed by value, required by language rules. It is simply a "handle"
  // to elements allocated "elsewhere"

  std::cout << "v[2] = " << v[2] << std::endl;

  Vector v1{3};  // calls the initializer list constructor with a single element 3

  Vector v2 = {3};  // calls the initializer list constructor with a single element 3

  Vector v3(3);  // calls the size constructor

  // What if I want to assign a new list of values to an existing Vector Object

  std::cout << "v[2] before = " << v[2] << ", v[4] before = " << v[4] << std::endl;
  v = {7.7, 8.8, 9.9};
  // warn: no assignment operator, so compiler generates a default assignment
  // operatos that performs a shallow copy of the elem pointer
  // causes double free runtime error when both v and the temporary object are destroyed
  // operator= must be defined to perform the deep copy of the elem array

  std::cout << "v[2] = " << v[2] << ", v[4] = " << v[4] << std::endl;

  return 0;
}