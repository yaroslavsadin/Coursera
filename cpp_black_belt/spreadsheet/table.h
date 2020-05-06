#pragma once
#include <vector>
#include <string>

template<typename T>
class Table {
private:
    using Storage = std::vector<std::vector<T>>;
public:
    class iterator {
    public:
        friend class Table; 
        iterator(Storage* storage, int row, int col)
        : storage(storage), row(row), col(col) {}
        T& operator*() {
            return (*storage)[row][col];
        }
        const T& operator*() const {
            return (*storage)[row][col];
        }
        iterator operator++(int) {
            if(++col == (*storage)[row].size()) {
                col = 0;
                if(++row == storage->size()) {
                    return *this; // past-the-end
                } else {
                    while((*storage)[row].empty() && row != storage->size()) {
                        row++;
                    }
                    if(row == storage->size()) {
                        return *this; // past-the-end;
                    }
                }
            }
            return *this;
        }
        bool operator==(iterator other) const {
            return (storage == other.storage) && (row == other.row) && (col == other.col);
        }
        bool operator!=(iterator other) const {
            return !(*this == other);
        }
    private:
        Storage* storage;
        int row;
        int col;
    };

    Table() : storage(), row_count(0), col_count(0), 
    invalid(&storage,-1,-1), begin_(invalid), end_(invalid) {}

    iterator begin() {
        return begin_;
    }
    iterator end() {
        return end_;
    }

    // const_iterator begin() const {
    //     return begin();
    // }
    // const_iterator end() const {
    //     return end_;
    // }

    // const_iterator cbegin() const {
    //     return begin_;
    // }
    // const_iterator cend() const {
    //     return end_;
    // }

    template<typename ElemType>
    void SetElement(int row, int col, ElemType&& elem) {
        static_assert(std::is_same_v<std::decay_t<T>,std::decay_t<ElemType>>);
        if(row_count < row + 1) {
            storage.resize(row + 1);
            row_count = storage.size();
        }
        if(storage[row].size() < col + 1) {
            storage[row].resize(col + 1);
            col_count = std::max(col_count,storage[row].size());
        }
        storage[row][col] = std::forward<T>(elem);

        if(row < begin_.row || (row == begin_.row && col < begin_.col) || begin_ == invalid) {
            begin_.row = row;
            begin_.col = col;
        }
        if(row > end_.row || (row == end_.row && col > end_.col) || end_ == invalid) {
            end_.row = row;
            end_.col = col;
            end_++;
        }
    }

    void InsertRows(size_t before, size_t count = 1) {
        if(row_count - 1 < before) {
            storage.resize(before + 1);
        }
        auto it = storage.begin() + before;
        while(count--) {
            it = storage.emplace(it);
            row_count++;
            if(begin_ != invalid && begin_.row >= before) {
                begin_.row ++;
            }
            if(end_ != invalid) {
                end_.row ++;
            }
        }
    }

    void InsertCols(size_t before, size_t count = 1) {
        for(auto& row : storage) {
            if(row.size() - 1 < before) {
                row.resize(before + 1);
            }
            auto it = row.begin() + before;
            auto count_ = count;
            while(count_--) {
                it = row.emplace(it);
            }
        }
        col_count += count;
        if(begin_ != invalid && begin_.col >= before) {
            begin_.col += count;
        }
    }

    void DeleteRows(size_t first, size_t count = 1) {

    }

    void DeleteCols(size_t first, size_t count = 1) {

    }

    size_t GetRowCount() const noexcept {
        return row_count;
    }

    size_t GetColCount() const noexcept {
        return col_count;
    }

private:
  Storage storage;
  size_t row_count;
  size_t col_count;
  iterator invalid;
  iterator begin_;
  iterator end_;
};