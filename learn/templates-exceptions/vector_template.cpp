#include <iostream>
#include <limits>
#include <memory>
#include <new>

// struct used to report some out of range error
struct out_of_range {};

int reasonable_size = std::numeric_limits<int>::max();

// generic class for memory allocation
template <typename T>
struct Allocator {
  T* allocate(int n)  // allocate  space for n objects of type T
  {
    if (n <= 0) return nullptr;

    // operator new returns void*, so we cast it to T*. it allocates (n*size of T) bytes
    void* p = ::operator new(n * sizeof(T));
    return static_cast<T*>(p);
  }

  void deallocate(T* p, int n)  // deallocate n objects of type T starting at p
  {
    // use global operator delete
    ::operator delete(p);
  }
};

// RAII pattern - "resource manager" that ensures memory is returned to the system
template <typename T, typename A = Allocator<T>>
struct Vector_rep {
  A alloc;    // allocator
  int sz;     // no of elements
  T* elem;    // start of allocation
  int space;  // amount of alloc space

  Vector_rep(const A& a, int n) : alloc{a}, sz{n}, elem{alloc.allocate(n)}, space{n} {}

  ~Vector_rep() { alloc.deallocate(elem, space); }
};

template <typename T, typename A = Allocator<T>>  // for all types T. Also called type generator
class Vector {
  Vector_rep<T, A> r;

 public:
  Vector() : r{A{}, 0} {}

  explicit Vector(int s) : r{A{}, s} {
    // int i = 0;
    // try {
    //   for (int i = 0; i < r.sz; ++i) {
    //     // placement new -> initializes memory at r.elem[i] without allocating more memory
    //     new (&r.elem[i]) T();
    //   }
    // } catch (...) {
    //   // if one fails, destroy all previously constructed elements
    //   for (int j = 0; j < i; ++j) {
    //     r.elem[j].~T();
    //   }
    //   throw;
    // }

    // this is a standard helper that handles the try-catch for us
    std::uninitialized_fill(r.elem, r.elem + r.sz, T{});
  }

  Vector(std::initializer_list<T>);             // list constructor
  Vector& operator=(std::initializer_list<T>);  // list assignment

  Vector(const Vector&);  // copy costructor

  // copy assignment
  Vector<T>& operator=(const Vector<T>& arg);

  Vector(Vector&&);             // move constructor
  Vector& operator=(Vector&&);  // move assignment

  ~Vector() {
    for (int i = 0; i < r.sz; ++i) {
      // explicitly call the destructor
      r.elem[i].~T();
    }
    // r's destructor will now run and free the raw memory
  }

  T& at(int n);              // checked access
  const T& at(int n) const;  // checked access

  T& operator[](int n) { return r.elem[n]; }              // unchecked access
  const T& operator[](int n) const { return r.elem[n]; }  // unchecked access

  int size() const { return r.sz; }
  int capacity() const { return r.space; }

  void resize(int sz, T def = T{});
  void push_back(const T& d);
  void reserve(int newalloc);

  T* begin() const { return r.elem; }
  T* end() const { return r.elem + r.sz; }
};

template <typename T>
bool operator==(const Vector<T>&, const Vector<T>&);

template <typename T>
bool operator!=(const Vector<T>&, const Vector<T>&);

template <typename T, typename A>
// copy assignment
Vector<T>& Vector<T, A>::operator=(const Vector<T>& arg) {
  if (this == &arg) return *this;  // self-assignment check

  // reuse existing memory => very fast
  if (arg.size() <= r.space) {
    std::move(arg.r.elem, arg.r.elem + arg.size(), r.elem);
    std::destroy(r.elem + arg.size(), r.elem + size());
    return *this;
  }

  // if not enough space, use copy-and-swap
  auto tmp = arg;         // copy all elements
  std::swap(*this, tmp);  // swap elements: strong guarantee
  return *this;
}

template <typename T, typename A>
T& Vector<T, A>::at(int n) {
  if (n < 0 || r.sz <= n) throw out_of_range();
  return r.elem[n];
}

template <typename T, typename A>
const T& Vector<T, A>::at(int n) const {
  if (n < 0 || r.sz <= n) throw out_of_range();
  return r.elem[n];
}

template <typename T, typename A>
void Vector<T, A>::reserve(int newalloc) {
  if (newalloc <= r.space) return;

  Vector_rep<T, A> b{r.alloc(), newalloc};
  std::uninitialized_move(r.elem, r.elem + r.sz, b.elem);  // move elements into uninitialized space
  std::destroy(r.elem, r.elem + r.sz);
  std::swap(r, b);
}

template <typename T, typename A>
void Vector<T, A>::push_back(const T& val) {
  reserve(r.space == 0 ? 8 : 2 * r.space);
  std::construct_at(&r.elem[r.sz], val);
  ++r.sz;
}

template <typename T, typename A>
void Vector<T, A>::resize(int newsize, T val) {
  reserve(newsize);
  if (r.sz < newsize) std::uninitialized_fill(&r.elem[r.sz], &r.elem[newsize], val);
  if (newsize < r.sz) std::destroy(&r.elem[newsize], &r.elem[r.sz]);
  r.sz = newsize;
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