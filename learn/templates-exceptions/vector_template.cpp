#include <iostream>
#include <limits>
#include <memory>

// struct used to report some out of range error
struct out_of_range {};

int reasonable_size = std::numeric_limits<int>::max();

template <typename T>
class Allocator {
 public:
  T* allocate(int n)  // allocate  space for n objects of type T
  {
    // a really fancy allocator code
    return nullptr;
  }

  void deallocate(T* p, int n)  // deallocate n objects of type T starting at p
  {
    // an even more fancy deallocator code
  }
};

template <typename T, typename A = Allocator<T>>  // for all types T. Also called type generator
class Vector {
  A alloc;    // member function allocate/deallocate handles memory
  int sz;     // the size
  T* elem;    // a pointer to the elements
  int space;  // size + free space

 public:
  Vector() : sz{0}, elem{nullptr}, space{0} {}

  explicit Vector(int s) : sz{s}, elem{new T[s]}, space{s} {
    for (int i = 0; i < sz; ++i) {
      elem[i] = 0;  // elements are initialized
    }
  }

  Vector(std::initializer_list<T>);             // list constructor
  Vector& operator=(std::initializer_list<T>);  // list assignment

  Vector(const Vector&);             // copy costructor
  Vector& operator=(const Vector&);  // copy assignment

  Vector(Vector&&);             // move constructor
  Vector& operator=(Vector&&);  // move assignment

  ~Vector() { delete[] elem; }  // destructor

  T& at(int n);              // checked access
  const T& at(int n) const;  // checked access

  T& operator[](int n) { return elem[n]; }              // unchecked access
  const T& operator[](int n) const { return elem[n]; }  // unchecked access

  int size() const { return sz; }
  int capacity() const { return space; }

  void resize(int sz, T def = T{});
  void push_back(const T& d);
  void reserve(int newalloc);

  T* begin() const { return elem; }
  T* end() const { return elem + sz; }
};

template <typename T>
bool operator==(const Vector<T>&, const Vector<T>&);

template <typename T>
bool operator!=(const Vector<T>&, const Vector<T>&);

template <typename T, typename A>
T& Vector<T, A>::at(int n) {
  if (n < 0 || sz <= n) throw out_of_range();
  return elem[n];
}

template <typename T, typename A>
const T& Vector<T, A>::at(int n) const {
  if (n < 0 || sz <= n) throw out_of_range();
  return elem[n];
}

template <typename T, typename A>
void Vector<T, A>::reserve(int newalloc) {
  if (newalloc <= space) return;

  T* p = alloc.allocate(newalloc);
  std::uninitialized_move(elem, &elem[sz], p);  // move elements into uninitialized space
  std::destroy(elem, &elem[space]);
  alloc.deallocate(elem, capacity());
  elem = p;
  space = newalloc;
}

template <typename T, typename A>
void Vector<T, A>::push_back(const T& val) {
  reserve(space == 0 ? 8 : 2 * space);
  std::construct_at(&elem[sz], val);
  ++sz;
}

template <typename T, typename A>
void Vector<T, A>::resize(int newsize, T val) {
  reserve(newsize);
  if (sz < newsize) std::uninitialized_fill(&elem[sz], &elem[newsize], val);
  if (newsize < sz) std::destroy(&elem[newsize], &elem[sz]);
  sz = newsize;
}

// struct TestStruct {
//   int a;
//   double b;
//   std::string c;
//   TestStruct() : a{0}, b{0.0}, c{""} {}
// };

class No_default {
 public:
  No_default(int num)  // the only constructor
  {
    std::cout << "No fault number is: " << num;
  }
};

void print_some(Vector<int>& v) {
  int i = -1;
  while (std::cin >> i && i != -1) {
    try {
      std::cout << "\n v[" << i << "] = " << v.at(i);
    } catch (const out_of_range&) {  // catch by reference of the type
      std::cout << "\n bad index: " << i;
    }
  }
}

int main() {
  // generalizing vectors
  // Vector<double> v1;
  // v1.resize(100);       // add 100 copied of double{}
  // v1.resize(200, 0.0);  // add 100 copies of 0.0
  // v1.resize(300, 1.0);  // add 100 copies of 1.0

  // Vector<No_default> v2(10);  // error: no No_default()
  // Vector<No_default> v3;          // ok: no elements
  // v3.resize(100, No_default{2});  // ok: not calling default constructor No_default()
  // v3.resize(200);  // error: no No_default()

  Vector<int> v4(10);

  v4.at(0) = 1;
  v4.at(1) = 2;
  v4.at(2) = 3;
  v4.at(5) = 8;

  print_some(v4);

  // allocators
  return 0;
}