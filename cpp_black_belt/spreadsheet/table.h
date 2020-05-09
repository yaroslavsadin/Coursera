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
        static_assert(sizeof(uint16_t) * 2 == sizeof(uint32_t));

        size_t operator()(Position p) const noexcept {
            return size_t(p.row) << 16 | p.col;
        }
    };
    using Storage = std::unordered_map<Position,std::shared_ptr<T>,PositionHash>;

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

    T* GetCell(Position pos) const {
        if(active_cells.count(pos)) {
            return active_cells.at(pos).get();
        }
        return nullptr;
    }

    template<typename ElemType>
    void SetCell(Position pos, ElemType&& data) {
        static_assert(std::is_same_v<std::decay_t<T>,std::decay_t<ElemType>>);
        UpdateSize(pos);
        active_cells[pos] = std::make_shared<ElemType>(std::forward<ElemType>(data));
    }

    void SetCell(Position pos, std::shared_ptr<T> data) {
        UpdateSize(pos);
        active_cells[pos] = std::move(data);
    }

    void ClearCell(Position pos) {
        if(active_cells.count(pos)) {
            active_cells.erase(pos);
        }
    }

    void InsertRows(size_t before, size_t count = 1) {
        if(before < size.rows) {
            Storage temp;
            size = {0,0};
            for(auto [pos_,cell] : active_cells) {
                auto pos = pos_;
                if(pos.row >= before) {
                    pos.row += count;
                }
                temp[pos] = std::move(cell);
                UpdateSize(pos);
            }
            active_cells.swap(temp);
        }
    }

    void InsertCols(size_t before, size_t count = 1) {
        if(before < size.cols) {
            Storage temp;
            size = {0,0};
            for(auto [pos_,cell] : active_cells) {
                auto pos = pos_;
                if(pos.col >= before) {
                    pos.col += count;
                }
                temp[pos] = std::move(cell);
                UpdateSize(pos);
            }
            active_cells.swap(temp);
        }
    }

    void DeleteRows(size_t first, size_t count = 1) {
        if(first < size.rows) {
            Storage temp;
            size = {0,0};
            for(auto [pos_,cell] : active_cells) {
                auto pos = pos_;
                if(pos.row < first) {
                    temp[pos] = std::move(cell);
                }
                if(pos.row >= first + count) {
                    pos.row -= count;
                    temp[pos] = std::move(cell);
                }
                UpdateSize(pos);
            }
            active_cells.swap(temp);
        }
    }

    void DeleteCols(size_t first, size_t count = 1) {
        if(first < size.cols) {
            size = {0,0};
            Storage temp;
            for(auto [pos_,cell] : active_cells) {
                auto pos = pos_;
                if(pos.col < first) {
                    temp[pos] = std::move(cell);
                }
                if(pos.col >= first + count) {
                    pos.col -= count;
                    temp[pos] = std::move(cell);
                }
                UpdateSize(pos);
            }
            active_cells.swap(temp);
        }
    }

    size_t GetRowCount() const noexcept {
        return size.rows;
    }

    size_t GetColCount() const noexcept {
        return size.cols;
    }

    std::map<Position,T*> GetSorted() const {
        std::map<Position,T*> res;
        for(auto [pos,cell] : active_cells) {
            res[pos] = cell.get();
        }
        return res;
    }

    // void Print(std::ostream& os) const {
    //     std::vector<std::vector<T*>> printable;
    //     printable.resize(printable_size.rows);
    //     for(const auto& row : printable) {
    //         row.resize(printable_size.cols);
    //     }
    //     for(auto [pos,cell] : active_cells) {
    //         printable[pos.row][pos.col] = cell.get();
    //     }
    //     for(const auto& row : printable) {
    //         for(auto* item : row) {
    //             if(item) {
    //                 os << item;
    //             }
    //         }
    //     }
    // }
private:
    void UpdateSize(Position pos) {
        if(size.rows < pos.row + 1) {
            size.rows = pos.row + 1;
        }
        if(size.cols < pos.col + 1) {
            size.cols = pos.col + 1;
        }
    }
};