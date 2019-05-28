#include <vector>
#include <string>

int main()
{
    std::vector<std::string> v(10);
    for (auto& e : v)
        e = "hello";
}