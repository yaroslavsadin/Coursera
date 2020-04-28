#include "sheet.h"
#include <algorithm>
#include "cell.h"

static inline void CheckPosition(Position pos, std::string message) {
    if(!pos.IsValid()) {
        throw InvalidPositionException("");
    }
}

void Sheet::SetCell(Position pos, std::string text){
    CheckPosition(pos, __FUNCTION__);
    if(storage.size() <= pos.row) {
        storage.resize(pos.row + 1);
    }
    if(storage[pos.row].size() <= pos.col) {
        storage[pos.row].resize(pos.col + 1);
    }
    storage[pos.row][pos.col] = std::make_unique<Cell>(text);
}
const ICell* Sheet::GetCell(Position pos) const{
    CheckPosition(pos, __FUNCTION__);
    if(storage.size() <= pos.row || storage[pos.row].size() <= pos.col) {
        return nullptr;
    } else {
        return storage.at(pos.row).at(pos.col).get();
    }
}
ICell* Sheet::GetCell(Position pos){
    CheckPosition(pos, __FUNCTION__);
    if(storage.size() <= pos.row || storage[pos.row].size() <= pos.col) {
        return nullptr;
    } else {
        return storage.at(pos.row).at(pos.col).get();
    }
}
void Sheet::ClearCell(Position pos){
    CheckPosition(pos, __FUNCTION__);
    if(storage.size() > pos.row && storage[pos.row].size() > pos.col) {
        storage[pos.row][pos.col].reset();
    }
}
void Sheet::InsertRows(int before, int count){
}
void Sheet::InsertCols(int before, int count){    
}
void Sheet::DeleteRows(int first, int count){
}
void Sheet::DeleteCols(int first, int count){
}
Size Sheet::GetPrintableSize() const{
    size_t cols = (storage.size()) ? std::max_element(storage.begin(),storage.end(),
    [](const auto& v1, const auto& v2){
        return v1.size() < v2.size();
    })->size() : 0;
    return Size{storage.size(),cols};
}
void Sheet::PrintValues(std::ostream& output) const{
}
void Sheet::PrintTexts(std::ostream& output) const{
}

std::unique_ptr<ISheet> CreateSheet() {
    return std::make_unique<Sheet>();
}