#include "sheet.h"
#include <algorithm>
#include "cell.h"
#include <cassert>

static inline void CheckPosition(Position pos, std::string message) {
    if(!pos.IsValid()) {
        throw InvalidPositionException(message);
    }
}

void Sheet::SetCell(Position pos, std::string text){
    CheckPosition(pos, __FUNCTION__);
    storage.SetCell(pos.row,pos.col,Cell(*this,text));
    for(auto pos : storage[pos.row][pos.col]->GetReferencedCells()) {
        if(storage.GetCell(pos.row,pos.col) == nullptr) {
            storage.SetCell(pos.row,pos.col,Cell(*this,""));
        }
    }
}
const ICell* Sheet::GetCell(Position pos) const{
    CheckPosition(pos, __FUNCTION__);
    return static_cast<ICell*>(storage.GetCell(pos.row,pos.col));
}
ICell* Sheet::GetCell(Position pos){
    CheckPosition(pos, __FUNCTION__);
    return static_cast<ICell*>(storage.GetCell(pos.row,pos.col));
}
void Sheet::ClearCell(Position pos){
    CheckPosition(pos, __FUNCTION__);
    storage.ClearCell(pos.row,pos.col);
}
void Sheet::InsertRows(int before, int count){
    if(before < storage.GetRowCount() && storage.GetRowCount() + count > Position::kMaxRows) {
        throw TableTooBigException(__FUNCTION__);
    } else {
        storage.InsertRows(before,count);
    }
}
void Sheet::InsertCols(int before, int count){    
    if(before < storage.GetColCount() && storage.GetColCount() + count > Position::kMaxCols) {
        throw TableTooBigException(__FUNCTION__);
    } else {
        storage.InsertCols(before,count);
    }
}

void Sheet::DeleteRows(int first, int count){
    storage.DeleteRows(first,count);
}
void Sheet::DeleteCols(int first, int count){
    storage.DeleteCols(first,count);
}
Size Sheet::GetPrintableSize() const{
    return {storage.GetRowCount(),storage.GetColCount()};
}
void Sheet::PrintValues(std::ostream& output) const{
}
void Sheet::PrintTexts(std::ostream& output) const{
}

std::unique_ptr<ISheet> CreateSheet() {
    return std::make_unique<Sheet>();
}