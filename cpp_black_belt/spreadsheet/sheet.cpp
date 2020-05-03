#include "sheet.h"
#include <algorithm>
#include "cell.h"
#include <cassert>

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

bool Sheet::CellExists(Position pos) const {
    return 
        storage.size() > pos.row && 
        storage[pos.row].size() > pos.col;
}

static inline void CheckPosition(Position pos, std::string message) {
    if(!pos.IsValid()) {
        throw InvalidPositionException(message);
    }
}

void Sheet::SetCell(Position pos, std::string text){
    CheckPosition(pos, __FUNCTION__);
    Extend(pos.row + 1,pos.col + 1);
    storage[pos.row][pos.col] = std::make_unique<Cell>(*this,text);
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
    if(storage.size() + count > Position::kMaxRows) {
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
    if(storage.size() + count > Position::kMaxCols) {
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
}
void Sheet::DeleteCols(int first, int count){
}
Size Sheet::GetPrintableSize() const{
    return Size{NumRows(),NumCols()};
}
void Sheet::PrintValues(std::ostream& output) const{
}
void Sheet::PrintTexts(std::ostream& output) const{
}

std::unique_ptr<ISheet> CreateSheet() {
    return std::make_unique<Sheet>();
}