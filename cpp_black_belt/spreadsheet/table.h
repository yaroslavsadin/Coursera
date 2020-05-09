#pragma once
#include "common.h"
#include <vector>
#include <string>
#include <set>
#include <cassert>
#include <algorithm>
#include <unordered_map>
#include <iostream>

template<typename T>
class Table {
private:
    uint16_t row_count;
    uint16_t col_count;

    struct TableSize {
        uint16_t row;
        uint16_t col;
        bool operator==(const TableSize& other) const {
            return row == other.row && col == other.col;
        }
    };

    struct TableSizeHash {
        static_assert(sizeof(uint16_t) * 2 == sizeof(uint32_t));

        size_t operator()(TableSize p) const noexcept {
            return size_t(p.row) << 16 | p.col;
        }
    };
    std::unordered_map<TableSize,std::shared_ptr<T>,TableSizeHash> active_cells;
public:
    using const_iterator = typename std::unordered_map<TableSize,std::shared_ptr<T>,TableSizeHash>::const_iterator;

    Table() : row_count(0), col_count(0) {}

    const_iterator begin() const {
        return active_cells.cbegin();
    }
    const_iterator end() const {
        return active_cells.cend();
    }

    T* GetCell(size_t row_num, size_t col_num) const {
        if(active_cells.count({row_num,col_num})) {
            return active_cells.at({row_num,col_num}).get();
        }
        return nullptr;
    }

    template<typename ElemType>
    void SetCell(size_t row, size_t col, ElemType&& data) {
        static_assert(std::is_same_v<std::decay_t<T>,std::decay_t<ElemType>>);
        auto pos = TableSize{row,col};
        UpdateSize(pos);
        active_cells[pos] = std::make_shared<ElemType>(std::forward<ElemType>(data));
    }

    void SetCell(size_t row, size_t col, std::shared_ptr<T> data) {
        auto pos = TableSize{row,col};
        UpdateSize(pos);
        active_cells[pos] = std::move(data);
    }

    void ClearCell(size_t row, size_t col) {
        if(active_cells.count({row,col})) {
            active_cells.erase({row,col});
        }
    }

    void InsertRows(size_t before, size_t count = 1) {
        if(before < row_count) {
            row_count = 0;
            col_count = 0;
            std::unordered_map<TableSize,std::shared_ptr<T>,TableSizeHash> temp;
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
        if(before < col_count) {
            row_count = 0;
            col_count = 0;
            std::unordered_map<TableSize,std::shared_ptr<T>,TableSizeHash> temp;
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
        if(first < row_count) {
            row_count = 0;
            col_count = 0;
            std::unordered_map<TableSize,std::shared_ptr<T>,TableSizeHash> temp;
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
        if(first < col_count) {
            row_count = 0;
            col_count = 0;
            std::unordered_map<TableSize,std::shared_ptr<T>,TableSizeHash> temp;
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
        return row_count;
    }

    size_t GetColCount() const noexcept {
        return col_count;
    }

    // void Print(std::ostream& os) const {
    //     std::vector<std::vector<T*>> printable;
    //     printable.resize(row_count);
    //     for(const auto& row : printable) {
    //         row.resize(col_count);
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
    void UpdateSize(TableSize pos) {
        if(row_count < pos.row + 1) {
            row_count = pos.row + 1;
        }
        if(col_count < pos.col + 1) {
            col_count = pos.col + 1;
        }
    }
};