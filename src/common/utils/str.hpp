#include <string>
#include <type_traits>

template <typename T>
std::string str(const T& value) {
    if constexpr (std::is_same_v<T, std::string>) {
        return value;
    } else {
        return std::to_string(value);
    }
}
