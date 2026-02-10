#include <iostream>

class Vector {
  long int sz;   // number of elements
  double* elem;  // address of first element
  int space;     // number of elements plus free space

 public:
  Vector() : sz{0}, elem{nullptr}, space{0} {}  // default constructor

  // initializer list
  Vector(std::initializer_list<double> lst) : sz{lst.end() - lst.begin()}, elem{new double[sz]} {
    std::copy(lst.begin(), lst.end(), elem);
  }

  ~Vector() { delete[] elem; }

  Vector& operator=(const Vector& a);  // like a copy constructor, but deal with old elements

  Vector(Vector&& arg);             // move constructor
  Vector& operator=(Vector&& arg);  // move assignment operator

  double& operator[](int i) { return elem[i]; }

  const double& operator[](int i) const { return elem[i]; }

  bool operator==(const Vector& other) const {
    if (this->size() != other.size()) return false;

    for (int i = 0; i < other.size(); i++) {
      if (this->elem[i] != other[i]) return false;
    }

    return true;
  }

  bool operator!=(const Vector& other) const { return !(*this == other); }

  void reserve(int newalloc);

  void resize(int newsize);

  int size() const { return sz; }

  void push_back(double d);

  double* begin() const { return elem; }
  double* end() const { return elem + sz; }
};

// define copy operation
Vector& Vector::operator=(const Vector& a) {
  // self-assignment
  if (this == &a) return *this;

  // if I have enough space in my current container
  if (a.sz <= space) {
    for (int i = 0; i < a.sz; ++i) {
      elem[i] = a.elem[i];
    }
    sz = a.sz;
    return *this;
  }

  // allocate new space if the container doesn't already have the capacity
  double* p = new double[a.sz];
  for (int i = 0; i < a.sz; ++i) {
    p[i] = a.elem[i];
  }
  delete[] elem;

  space = sz = a.sz;
  elem = p;
  return *this;
}

// define move constructor: copy resources from arg to this object
Vector::Vector(Vector&& arg) : sz{arg.sz}, elem{arg.elem}, space{arg.space} {
  std::cout << "Move constructor called\n";
  // leave arg in a state that is safe for destruction
  arg.sz = 0;
  arg.elem = nullptr;
  arg.space = 0;
}

// define move assignment operator: move arg to this object
Vector& Vector::operator=(Vector&& arg) {
  std::cout << "Move assignment operator called\n";
  // protect against self-assignment
  if (this != &arg) {
    delete[] elem;  // free old resources
    sz = arg.sz;
    elem = arg.elem;
    space = arg.space;
    // leave arg in a state safe for destruction
    arg.sz = 0;
    arg.elem = nullptr;
    arg.space = 0;
    std::cout << "this address: " << this << "\n";
    std::cout << "*this address: " << &(*this) << "\n";
  }
  return *this;  // return a self-reference
}

void Vector::reserve(int newalloc) {
  if (newalloc <= space) return;     // never decrease allocation
  double* p = new double[newalloc];  // allocate new space
  for (int i = 0; i < sz; i++) {
    p[i] = elem[i];  // copy old elements
  }
  delete[] elem;  // free old space
  elem = p;       // set elem to point to the new space
  space = newalloc;
}

void Vector::resize(int newsize) {
  reserve(newsize);
  for (int i = sz; i < newsize; ++i) {
    elem[i] = 0;
  }
  sz = newsize;
}

void Vector::push_back(double d) {
  // increase vector size by one, initialize new element with d
  if (space == 0)
    reserve(8);  // start with space for 8 elements
  else if (sz == space)
    reserve(2 * space);  // double available space

  elem[sz] = d;
  sz++;
}

int main() {
  Vector v{1.2, 4.1, 0.0, 5.0};

  std::cout << "vector elements are: ";

  for (int i = 0; i < v.size(); i++) {
    std::cout << "\n" << v[i];
  }

  std::cout << std::endl;

  Vector v2;

  std::cout << "Comparison of 2 vectors: (v2 == v) =  " << (v2 == v);

  return 0;
}