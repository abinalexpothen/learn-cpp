#include <iostream>

class Vector {
  int sz;        // number of elements
  double* elem;  // address of first element
  int space;     // number of elements plus free space

 public:
  Vector() : sz{0}, elem{nullptr}, space{0} {}  // default constructor

  ~Vector() { delete[] elem; }

  Vector(Vector&& arg);             // move constructor
  Vector& operator=(Vector&& arg);  // move assignment operator

  /* This funny && notation is called an rvalue reference */

  void reserve(int newalloc);

  void push_back(double d);
};

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

void Vector::push_back(double d) {
  // increase vector size by one, initialize new element with d
  if (space == 0)
    reserve(8);  // start with space for 8 elements
  else if (sz == space)
    reserve(2 * space);  // double available space

  elem[sz] = d;
  sz++;
}

Vector fill(std::istream& in) {
  Vector res;
  std::cout << "Address of res in fill(): " << &res << "\n";
  for (double d; in >> d;) {
    res.push_back(d);
  }
  return std::move(res);  // generally, this tells the compiler to move rather than copy.
  // in this context, this was used to force a move because the compiler would apply copy elision
  // and optimize away the move anyway.
}

Vector* fill2(std::istream& is) {
  Vector* res = new Vector;
  for (double x; is >> x;) res->push_back(x);
  return res;
}

int main() {
  std::cout << "Enter numbers to fill the vector (non-number to stop): ";
  Vector v = fill(std::cin);

  std::cout << "Address of v: " << &v << "\n";
  std::cout << "Vector filled.\n";

  /* Notice that the address of v and res are the same. Compiler is doing a copy elision
  optimization.

  In order to make sure this works even without copy elision, we need to implement a move
  constructor and a move assignment operator.
  */

  // flawed conventional way
  std::cout << "Using flawed conventional way to fill vector\n";
  Vector* v2 = fill2(std::cin);
  delete v2;  // <-- avoid memory leak, this is not easy to do in large programs
  std::cout << "Vector filled using flawed conventional way\n";
  return 0;
}
