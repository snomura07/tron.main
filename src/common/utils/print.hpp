#include <iostream>
#include <string>
#include <utility>

template <typename... Args>
void print(Args&&... args) {
    (std::cout << ... << std::forward<Args>(args));
    std::cout << std::endl;
}
