#include <iostream>
#include <memory>
#include <vector>

std::vector<int> make_vec() {
  std::vector<int> res;
  // fill the vector with data -> this may throw an exception
  res = {1, 2, 3, 4, 5};
  return res;  // return a pointer
}

int main() {
  std::cout << "\n Resource Management Pointers";
  // this invokes move constructor, which will get the ownership of the elements out of the function
  auto v = make_vec();

  std::cout << "\n Printing vectors that were move-constructed";

  for (auto& elem : v) {
    std::cout << "\n Vector Element: " << elem;
  }

  std::unique_ptr<double> o = std::make_unique<double>();
  std::cout << "\n Value pointed to by the unique_ptr o -> " << *o;
  std::unique_ptr<int[]> p = std::make_unique<int[]>(5);
  std::cout << "\n Value pointed to by the unique_ptr p[2] -> " << p[2];

  // std::unique_ptr<int[]> q = p; // illegal - multiple unique_pointer cannot point to same object

  std::shared_ptr<int> r = std::make_shared<int>();
  std::shared_ptr<int> s = r;  // shared_ptr allows multiple pointers to share the same object

  std::cout << "\n Value pointed to by the shared_ptr s -> " << *s;

  return 0;
}