#include <iostream>

template <typename T>  // for all types T. Also called type generator
class Vector {
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

  T& operator[](int n) { return elem[n]; }  // access: return reference
  const T& operator[](int n) const { return elem[n]; }

  int size() const { return sz; }
  int capacity() const { return space; }

  void resize(int newsize);
  void push_back(const T& d);
  void reserve(int newalloc);

  T* begin() const { return elem; }
  T* end() const { return elem + sz; }
};

template <typename T>
bool operator==(const Vector<T>&, const Vector<T>&);

template <typename T>
bool operator!=(const Vector<T>&, const Vector<T>&);

int main() {
  // Add stuff to test template class once the functions are implemented
  return 0;
}