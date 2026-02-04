#include <iostream>

class Vector {
  int sz;        // size of vector element
  double* elem;  // pointer to vector elements

 public:
  Vector(int s) : sz(s), elem(new double[s]) {
    for (int i = 0; i < sz; ++i) elem[i] = 0;
  }

  ~Vector() { delete[] elem; }

  double& operator[](int n) { return elem[n]; }
  const double& operator[](int n) const { return elem[n]; }

  int size() const { return sz; }
};

int main() {
  std::cout << "Essential operations example\n";

  Vector v(3);

  for (int i = 0; i < v.size(); ++i) {
    v[i] = i;
    std::cout << "Address of vector element at index " << i << ": " << &v[i] << " | Value: " << v[i]
              << "\n";
  }

  v[0] = 42.0;
  std::cout << "After modification, v[0]: " << v[0] << "\n";

  const Vector cv = v;
  std::cout << "Accessing const vector cv[1]: " << cv[1] << "\n";

  // cv[1] = 100.0;  // This line should cause a compilation error

  return 0;
}