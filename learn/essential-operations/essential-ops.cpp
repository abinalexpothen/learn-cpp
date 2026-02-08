/*

When do we need a:

default constructor -> if we can establish the invariant for the class with a meaningful and
obvious default.

destructor -> if it acquires resources that you "get from somewhere" and need to give back once
finished.

example: memory from free store using new and have to give back to free store with delete
or delete[] or files, if you  open files close it, locks, thread handles, sockets, if class has
pointers and references, it often needs destructor and copy operations.

a class that needs destructor almost always also needs a copy and/or move operations: if it acquires
resources, shallow copy is wrong.

A base class for which a derived class may have a destructor needs a virtual destructor.

Rule of zero: if you don't need to, don't define any essential operation.
Rule of all: if you need to define one essential operation, define all.

*/

#include <iostream>
#include <vector>

class Vector {
 public:
  // default constructor
  Vector() : sz{0}, elem{nullptr} {}

 private:
  int sz;        // number of elements
  double* elem;  // address of first element
};

class complex {
 public:
  explicit complex(double);
  // explicit provides only the usual construction semantics and not the
  // implicit conversions: used to avoid surprising conversions. Ideally, constructors
  // should have been explicit by default, but language designers wanted to allow for
  // implicit conversions for single-argument constructors, so they added the explicit
  // keyword to allow programmers to opt out of that behavior.
  complex(double, double);

  void print_complex() const { std::cout << "real: " << re << ", imag: " << im << "\n"; }

 private:
  double re, im;
};

complex::complex(double r) : complex{r, 0} {}  // delegating constructor

complex::complex(double r, double i) : re{r}, im{i} {
  // initialize real and imaginary parts
}

// create a class x that leaves a trace of its operations
struct X {
  int val;

  void out(const std::string& s, int nv) {
    std::cout << this << "->" << s << ": " << val << " (" << nv << ")" << "\n";
  }

  X() {
    out("default constructor", 0);
    val = 0;
  };

  X(int x) {
    out("constructor with int argument", x);
    val = x;
  }

  X(const X& x) {
    out("copy constructor", x.val);
    val = x.val;
  }

  X(X&& x) {
    out("move constructor", x.val);
    val = x.val;
    x.val = 0;  // leave x in a state safe for destruction
  }

  X& operator=(const X& x) {
    out("copy assignment operator", x.val);
    val = x.val;
    return *this;  // return a self-reference
  }

  X& operator=(X&& x) {
    out("move assignment operator", x.val);
    val = x.val;
    x.val = 0;     // leave x in a state safe for destruction
    return *this;  // return a self-reference
  }

  ~X() { out("destructor", 0); }
};

X glob{2};

X copy(X a) {
  std::cout << "copy function called with argument: " << a.val << "\n";
  return a;  // returns a copy of a, which will invoke the move constructor
}

X copy2(X a) {
  std::cout << "copy2 function called with argument: " << a.val << "\n";
  X aa = a;  // copy constructor
  return aa;
}

X& ref_to(X& a) {
  std::cout << "ref_to function called with argument: " << a.val << "\n";
  return a;  // returns a reference to a, no copy is made
}

X* make(int i) {
  X a(i);           // constructor with int argument
  return new X(a);  // copy constructor, returns a pointer to a new X object on the heap
}

struct XX {
  X a;
  X b;
};

int main() {
  // complex z1 = 3.14; // error: no viable conversion from 'double' to 'complex' because the
  // constructor is explicit
  complex z1{3.14};  // direct initialization, calls the constructor with a single double argument
  z1.print_complex();

  // test class X to study the essential operations

  X loc{4};
  /* 4 is an int type. With the {}, I expect this will call the constructor with the int
   * argument and initialize the member variable inside X
   *
   * Observation-> correct.
   */

  X loc2{loc}; /* I expect a call to the copy constructor. Observation -> Correct.*/

  loc = X{5};
  /* I expect call to the int constructor and then a call to the copy assignment operator

  Observation -> Wrong, it calls the move assignment operator because X{5} is a temporary object */

  loc2 = copy(loc);
  /* I expect a call to the copy assignment operator

  Observation-> quite a complex series of ops. There's copy contructor first as loc value is
  transferred to a. Then as it's returned from a, there is a temporary variable created with move
  constructor and then the value gets assigned to the result through move assignment operator of
  loc2.*/

  loc2 = copy2(loc);

  /* I expect
  loc -> a: a copy constructor,
  a -> aa: copy constructor
  aa -> temporary variable: move constructor
  temporary var -> loc2: move assignment
  destructors for temporary, aa, and a in sequence

  Observation -> no temporary variable! Direct move from aa to loc2 through move assignment
  operator. Copy elision?
  */

  X loc3{6};
  X& r = ref_to(loc3);

  /*
  Expect:
  - loc3 constructor with int argument
  - no copy constructor as passed by reference.
  - no copy constructor as returned by reference

  Correct observation.
  */

  std::cout << "make function test\n";
  delete make(7);
  delete make(8);

  /*
  Expect:
  - constructor with int argument
  - copy constructor for the temporary object in new
  - returned pointer to the outside of function
  - destructor for the initially constructed object
  - destructor for the new object when it's deleted in main

  Correct.
  */

  std::vector<X> v(4);
  /*
  Expect:
  - default constructor for each of the 4 elements

  Correct.
  */

  XX loc4;
  /*
  Expect:
  - default constructor for 2 members of XX

  Correct.
  */

  std::cout << "Direct initialization of an object on the heap\n";

  X* p = new X{9};
  delete p;
  /*
  Expect:
  - constructor with int argument for new
  - destructor when delete called

  Correct.
  */

  std::cout << "Array of objects on the heap\n";

  X* pp = new X[5];  // array of X on free store
  delete[] pp;

  /*
  Expect:
  - default constructor for each of the 5 elements in the array
    - destructor for each of the 5 elements when delete[] called
  */

  return 0;
}