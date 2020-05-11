#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

struct Size {
  size_t rows = 0;
  size_t cols = 0;
};

void CleanRow(std::vector<int>& row) {
    auto it = find_if(row.rbegin(),row.rend(),
    [](const auto& rhs){
        return rhs != 0;
    });
    row.erase(it.base(),row.end());
}

int main() {
    std::vector<std::vector<int>> storage;
    storage.push_back({0,0,0,0,0});
    // storage.push_back({1,0,1});
    storage.push_back({0,0});
    storage.push_back({0});
    // storage.push_back({1,0,0,1,0,0,0,0,0,0});
    // storage.push_back({0});
    storage.push_back({0});
    storage.push_back({0});
    storage.push_back({0});
    // storage.push_back({0});

    Size size{0,0};

    auto r_it_row = storage.rbegin();
    while(r_it_row != storage.rend()) {
        auto it_row = r_it_row.base();
        auto& row = *r_it_row++;
        CleanRow(row);
        if(row.size()) {
            size.cols = std::max(size.cols,row.size());
        } else if(it_row == storage.end()) {
            storage.erase(prev(it_row));
        }
    }
    size.rows = storage.size();

    for(const auto& row : storage) {
        std::cout << "Row: ";
        for(auto cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << size.rows << ' ' << size.cols << std::endl;

    std::cerr << "Success";
}