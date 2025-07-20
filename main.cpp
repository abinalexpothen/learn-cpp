#include <iostream>
// import std; -> unsupported on mac gcc/clang right now

int main()
{
  std::cout << "Enter your first name, second name and age in years: ";
  std::string first_name;
  std::string second_name;
  int age = -1;
  std::cin >> first_name >> second_name >> age;
  std::cout << "Hello, " << first_name << " " << second_name << "!\n";
  std::cout << "You are " << age * 12 << " months old.\n";

  return 0;
}