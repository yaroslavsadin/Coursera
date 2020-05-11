#pragma once
#include "common.h"
#include <cassert>
#include <algorithm>
#include <deque>

template<typename T>
class Table {
private:
    using Storage = std::deque<std::deque<std::unique_ptr<T>>>;

    Size size;
    Storage storage;

    bool ContainsCell(Position pos) const {
        return storage.size() > pos.row && storage[pos.row].size() > pos.col;
    }
    void CleanRow(std::deque<std::unique_ptr<T>>& row) {
        auto it = find_if(row.rbegin(),row.rend(),
        [](const auto& rhs){
            return rhs != nullptr;
        });
        row.erase(it.base(),row.end());
    }
    void Normalize() {
        size = {0,0};
        auto r_it_row = storage.rbegin();
        while(r_it_row != storage.rend()) {
            auto it_row = r_it_row.base();
            auto& row = *r_it_row++;
            CleanRow(row);
            if(row.size()) {
                size.cols = std::max(static_cast<size_t>(size.cols),row.size());
            } else if(it_row == storage.end()) {
                storage.erase(prev(it_row));
            }
        }
        size.rows = storage.size();
    }
    void UpdateSize(Position pos) {
        if(size.rows < pos.row + 1) {
            size.rows = pos.row + 1;
        }
        if(size.cols < pos.col + 1) {
            size.cols = pos.col + 1;
        }
    }
public:
    Table() = default;

    const T* GetCell(Position pos) const {
        if(ContainsCell(pos)) {
            return storage[pos.row][pos.col].get();
        }
        return nullptr;
    }

    T* GetCell(Position pos) {
        if(ContainsCell(pos)) {
            return storage[pos.row][pos.col].get();
        }
        return nullptr;
    }

    void SetCell(Position pos, std::unique_ptr<T> data) {
        if(storage.size() <= pos.row) {
            storage.resize(pos.row + 1);
        }
        if(storage[pos.row].size() <= pos.col) {
            storage[pos.row].resize(pos.col + 1);
        }
        storage[pos.row][pos.col] = std::move(data);
        UpdateSize(pos);
    }

    void ClearCell(Position pos) {
        if(ContainsCell(pos)) {
            storage[pos.row][pos.col].reset();
            if(pos.row == size.rows - 1 || pos.col == size.cols - 1) {
                Normalize();
            }
        }
    }

    void InsertRows(int before, int count = 1) {
        if(before < size.rows) {
            for(size_t i = 0; i < static_cast<size_t>(count); i++) {
                storage.emplace(storage.begin() + before + i);
            }
        }
    }

    void InsertCols(int before, int count = 1) {
        if(before < size.cols) {
            for(auto& row : storage) {
                if(before < row.size()) {
                    for(size_t i = 0; i < static_cast<size_t>(count); i++) {
                        row.emplace(row.begin() + before + i);
                    }
                }
            }
        }
    }

    void DeleteRows(int first, int count = 1) {
        if(first < size.rows && storage.size()) {
            storage.erase(
                storage.begin() + first, 
                storage.begin() + first + std::min(static_cast<size_t>(count),storage.size())
            );
        }
    }

    void DeleteCols(int first, int count = 1) {
        if(first < size.cols) {
            for(auto& row : storage) {
                if(first < row.size()) {
                    row.erase(
                        row.begin() + first, 
                        row.begin() + first + std::min(static_cast<size_t>(count),row.size())
                    );
                }
            }
            Normalize();
        }
    }

    int GetRowCount() const noexcept {
        return size.rows;
    }

    int GetColCount() const noexcept {
        return size.cols;
    }

    const Storage& Get() const {
        return storage;
    }
    Storage& Get() {
        return storage;
    }
};