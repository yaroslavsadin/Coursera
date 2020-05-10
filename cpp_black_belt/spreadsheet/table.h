#pragma once
#include "common.h"
#include <cassert>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <iostream>

template<typename T>
class Table {
private:
    struct PositionHash {
        // We imply that row and col are both maxed to 16383 i.e. 16 bit
        size_t operator()(Position p) const noexcept {
            return size_t(p.row) << 16 | p.col;
        }
    };
    using Storage = std::unordered_map<Position,T,PositionHash>;

    Size size;
    Storage active_cells;
public:
    using const_iterator = typename Storage::const_iterator;

    Table() = default;

    const_iterator begin() const {
        return active_cells.cbegin();
    }
    const_iterator end() const {
        return active_cells.cend();
    }

    const T* GetCell(Position pos) const {
        if(active_cells.count(pos)) {
            return &active_cells.at(pos);
        }
        return nullptr;
    }

    T* GetCell(Position pos) {
        if(active_cells.count(pos)) {
            return &active_cells.at(pos);
        }
        return nullptr;
    }

    template<typename ElemType>
    void SetCell(Position pos, ElemType&& data) {
        static_assert(std::is_same_v<std::decay_t<T>,std::decay_t<ElemType>>);
        active_cells[pos] = std::forward<ElemType>(data);
        UpdateSize(pos);
    }

    void ClearCell(Position pos) {
        if(active_cells.count(pos)) {
            active_cells.erase(pos);
            size = {0,0};
            for(auto [pos_,cell] : active_cells) {
                UpdateSize(pos);
            }
        }
    }

    void InsertRows(int before, int count = 1) {
        if(before < size.rows) {
            Insert(&Position::row, before, count);
        }
    }

    void InsertCols(int before, int count = 1) {
        if(before < size.cols) {
            Insert(&Position::col, before, count);
        }
    }

    void DeleteRows(int first, int count = 1) {
        if(first < size.rows) {
            Delete(&Position::row, first, count);
        }
    }

    void DeleteCols(int first, int count = 1) {
        if(first < size.cols) {
            Delete(&Position::col, first, count);
        }
    }

    int GetRowCount() const noexcept {
        return size.rows;
    }

    int GetColCount() const noexcept {
        return size.cols;
    }

    std::vector<std::vector<const T*>> GetPrintable() const {
        std::vector<std::vector<const T*>> printable(size.rows,std::vector<const T*>(size.cols,nullptr));
        for(const auto& [pos,cell] : active_cells) {
            printable[pos.row][pos.col] = &cell;
        }
        return printable;
    }
private:
    void UpdateSize(Position pos) {
        if(size.rows < pos.row + 1) {
            size.rows = pos.row + 1;
        }
        if(size.cols < pos.col + 1) {
            size.cols = pos.col + 1;
        }
    }
    void Insert(int Position::* field, int before, int count) {
        Storage temp;
        size = {0,0};
        for(auto [pos_,cell] : active_cells) {
            auto pos = pos_;
            if(pos.*field >= before) {
                pos.*field += count;
            }
            temp[pos] = std::move(cell);
            UpdateSize(pos);
        }
        active_cells.swap(temp);
    }
    void Delete(int Position::* field, int first, int count) {
        Storage temp;
        size = {0,0};
        for(auto [pos_,cell] : active_cells) {
            auto pos = pos_;
            if(pos.*field < first) {
                temp[pos] = std::move(cell);
            }
            if(pos.*field >= first + count) {
                pos.*field -= count;
                temp[pos] = std::move(cell);
            }
            UpdateSize(pos);
        }
        active_cells.swap(temp);
    }
};