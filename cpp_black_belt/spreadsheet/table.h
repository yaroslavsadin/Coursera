#pragma once
#include "common.h"
#include <vector>
#include <string>
#include <set>
#include <cassert>
#include <iostream>

template<typename T>
class Table {
private:
    using Row = std::vector<std::shared_ptr<T>>;
    using Storage = std::vector<Row>;
public:
    using const_iterator = typename Storage::const_iterator;
    using row_iterator = typename Row::const_iterator;

    Table() : storage(), row_count(0), col_count(0) {}

    const_iterator begin() const {
        return storage.cbegin();
    }
    const_iterator end() const {
        return storage.cend();
    }

    const Row& GetRow(size_t row_num) const {
        assert(row_num < row_count);
        return storage[row_num];
    }

    const Row& operator[](size_t row_num) const {
        return GetRow(row_num);
    }

    T* GetCell(size_t row_num, size_t col_num) const {
        if(row_num < storage.size()) {
            if(col_num < storage[row_num].size()) {
                return storage[row_num][col_num].get();
            }
        }
        return nullptr;
    }

    template<typename ElemType>
    void SetCell(int row, int col, ElemType&& data) {
        static_assert(std::is_same_v<std::decay_t<T>,std::decay_t<ElemType>>);
        if(storage.size() < row + 1) {
            storage.resize(row + 1);
            row_count = storage.size();
        }
        if(storage[row].size() < col + 1) {
            storage[row].resize(col + 1);
            col_count = std::max(col_count,storage[row].size());
        }
        storage[row][col] = std::make_shared<ElemType>(std::forward<ElemType>(data));
    }

    void SetCell(int row, int col, std::shared_ptr<T> data) {
        if(storage.size() < row + 1) {
            storage.resize(row + 1);
            row_count = storage.size();
        }
        if(storage[row].size() < col + 1) {
            storage[row].resize(col + 1);
            col_count = std::max(col_count,storage[row].size());
        }
        storage[row][col] = std::move(data);
    }

    void ClearCell(int row, int col) {
        if(row < storage.size() && col < storage[row].size()) {
            storage[row][col].reset();
        }
    }

    void InsertRows(size_t before, size_t count = 1) {
        if(row_count && before < row_count ) {
            auto it = storage.begin() + before;
            while(count--) {
                it = storage.emplace(it);
                row_count++;
            }
        }
    }

    void InsertCols(size_t before, size_t count = 1) {
        if(before < col_count) {
            for(auto& row : storage) {
                if(row.size() && before < row.size()) {
                    auto it = row.begin() + before;
                    auto count_ = count;
                    while(count_--) {
                        it = row.emplace(it);
                        col_count++;
                    }
                }
            }
        }
    }

    void DeleteRows(size_t first, size_t count = 1) {
        if(first < row_count) {
            auto it_last = (first + count < storage.size()) ? storage.begin() + first + count : storage.end();
            size_t row_count_delta = std::distance(storage.begin() + first,it_last);
            storage.erase(storage.begin() + first,it_last);
            row_count -= row_count_delta;
        }
    }

    void DeleteCols(size_t first, size_t count = 1) {
        if(first < col_count) {    
            size_t erased_max = 0;
            for(auto& row : storage) {
                if(first < row.size()) {
                    auto it_last = (first + count < row.size()) ? row.begin() + first + count : row.end();
                    size_t col_count_delta = std::distance(row.begin() + first,it_last);
                    row.erase(row.begin() + first,it_last);
                    erased_max = std::max(erased_max,col_count_delta);
                }
            }
            col_count -= erased_max;
        }
    }

    size_t GetRowCount() const noexcept {
        return row_count;
    }

    size_t GetColCount() const noexcept {
        return col_count;
    }

    void Print(std::ostream& output) const {
        // for(const auto& row : storage) {
        //     for(auto val : row) {
        //         output << val->GetValue() << " | ";
        //     }
        //     output << '\n';
        // }
        // output << '\n';
    }

private:
    Storage storage;
    size_t row_count;
    size_t col_count;
};