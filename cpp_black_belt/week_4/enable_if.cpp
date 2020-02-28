#include <iostream>
#include <type_traits>

template<typename T, 
    typename std::enable_if_t<
        std::is_same_v<
            std::remove_reference_t<T>,
            std::string
        >,
        std::remove_reference_t<T>>* = nullptr
>
void print(T&& v) {
    std::cout << v << std::endl;
}

int main(void) {
    std::string s("test");
    print(s);
    print(42);
    return 0;
}