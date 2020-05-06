#include "sheet.h"
#include <algorithm>
#include "cell.h"
#include <cassert>

static inline void CheckPosition(Position pos, std::string message) {
    if(!pos.IsValid()) {
        throw InvalidPositionException(message);
    }
}

void Sheet::Extend(size_t rows, size_t cols) {
    if(NumRows() < rows) {
        storage.resize(rows);
    }
    if(NumRows() != 0) {
        cols = std::max(NumCols(),cols);
        for(auto& row : storage) {
            row.resize(cols);
        }
    }
}

void Sheet::InsertCell(Position pos) {
    CheckPosition(pos, __FUNCTION__);
    if(NumRows() < pos.row + 1) {
        storage.resize(pos.row + 1);
        size.rows = storage.size();
    }
    if(storage[pos.row].size() < pos.col + 1) {
        storage[pos.row].resize(pos.col + 1);
        size.cols = std::max(static_cast<size_t>(size.cols),storage[pos.row].size());
    }
}

bool Sheet::CellExists(Position pos) const {
    return storage.size() > pos.row && storage[pos.row].size() > pos.col;
}

void Sheet::SetCell(Position pos, std::string text){
    InsertCell(pos);
    storage[pos.row][pos.col] = std::make_unique<Cell>(*this,text);
    for(auto pos : storage[pos.row][pos.col]->GetReferencedCells()) {
        if(!CellExists(pos)) {
            SetCell(pos, "=0");
        }
    }
}
const ICell* Sheet::GetCell(Position pos) const{
    CheckPosition(pos, __FUNCTION__);
    if(CellExists(pos)) {
        return storage.at(pos.row).at(pos.col).get();
    }
    return nullptr;
}
ICell* Sheet::GetCell(Position pos){
    CheckPosition(pos, __FUNCTION__);
    if(CellExists(pos)) {
        return storage.at(pos.row).at(pos.col).get();
    }
    return nullptr;
}
void Sheet::ClearCell(Position pos){
    CheckPosition(pos, __FUNCTION__);
    if(CellExists(pos)) {
        storage[pos.row][pos.col].reset();
    }
}
void Sheet::InsertRows(int before, int count){
    if(NumRows() + count > Position::kMaxRows) {
        throw TableTooBigException(__FUNCTION__);
    } else {
        Extend(before + count + 1,NumCols());
        std::vector<std::vector<std::unique_ptr<ICell>>> temp(count);
        for(auto& row : temp) row.resize(NumCols());
        storage.insert(
            storage.begin()+before,
            std::make_move_iterator(temp.begin()),
            std::make_move_iterator(temp.end())
        );
        for(const auto& row : storage) {
            for(auto& cell : row) {
                cell->HandleInsertedRows(before,count);
            }
        }
    }
}
void Sheet::InsertCols(int before, int count){    
    if(NumCols() + count > Position::kMaxCols) {
        throw TableTooBigException(__FUNCTION__);
    } else {
        Extend(NumRows(),before + count + 1);
        for(auto& row : storage) {
            row.insert(row.begin()+before,nullptr);
        }
        for(const auto& row : storage) {
            for(auto& cell : row) {
                cell->HandleInsertedCols(before,count);
            }
        }
    }
}
void Sheet::DeleteRows(int first, int count){
    if(NumRows() >= first) {
        auto it_start = storage.begin() + first;
        auto it_end = (std::distance(storage.begin(),storage.end()) < count) ? 
        storage.begin() + first + count : storage.end();
        storage.erase(it_start,it_end);
    }
}
void Sheet::DeleteCols(int first, int count){
    if(NumCols() >= first) {
        for(auto& row : storage) {
            if(row.size() >= first) {
                auto it_start = row.begin() + first;
                auto it_end = (std::distance(row.begin(),row.end()) < count) ? 
                row.begin() + first + count : row.end();
                row.erase(it_start,it_end);
            }
        }
    }
}
Size Sheet::GetPrintableSize() const{
    return size;
}
void Sheet::PrintValues(std::ostream& output) const{
}
void Sheet::PrintTexts(std::ostream& output) const{
}

std::unique_ptr<ISheet> CreateSheet() {
    return std::make_unique<Sheet>();
}