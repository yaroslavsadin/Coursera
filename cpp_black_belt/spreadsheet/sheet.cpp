#include "sheet.h"
#include <algorithm>
#include "cell.h"
#include <cassert>

bool Size::operator==(const Size& rhs) const {
    return rows == rhs.rows && cols == rhs.cols;
}

static inline void CheckPosition(Position pos, std::string message) {
    if(!pos.IsValid()) {
        throw InvalidPositionException(message);
    }
}

void Sheet::SetCell(Position pos, std::string text){
    CheckPosition(pos, __FUNCTION__);
    auto item = std::make_shared<Cell>(*this,text);
    storage.SetCell(pos.row,pos.col,item);
    for(auto referenced : storage[pos.row][pos.col]->GetReferencedCells()) {
        if(storage.GetCell(referenced.row,referenced.col) == nullptr) {
            storage.SetCell(referenced.row,referenced.col,std::make_shared<Cell>(*this,""));
        }
        storage.GetCell(referenced.row,referenced.col)->Subscribe(item);
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
        for(const auto& row : storage) {
            for(auto cell : row) {
                if(cell) {  
                    cell->HandleInsertedRows(before,count);
                }
            }
        }
    }
}
void Sheet::InsertCols(int before, int count){    
    if(before < storage.GetColCount() && storage.GetColCount() + count > Position::kMaxCols) {
        throw TableTooBigException(__FUNCTION__);
    } else {
        storage.InsertCols(before,count);
        for(const auto& row : storage) {
            for(auto cell : row) {
                if(cell) {  
                    cell->HandleInsertedCols(before,count);
                }
            }
        }
    }
}

void Sheet::DeleteRows(int first, int count){
    storage.DeleteRows(first,count);
    for(const auto& row : storage) {
        for(auto cell : row) {
            if(cell) {  
                cell->HandleDeletedRows(first,count);
            }
        }
    }
}
void Sheet::DeleteCols(int first, int count){
    storage.DeleteCols(first,count);
    for(const auto& row : storage) {
        for(auto cell : row) {
            if(cell) {  
                cell->HandleDeletedCols(first,count);
            }
        }
    }
}
Size Sheet::GetPrintableSize() const{
    return {storage.GetRowCount(),storage.GetColCount()};
}
void Sheet::PrintValues(std::ostream& output) const{
    storage.Print(output);
}
void Sheet::PrintTexts(std::ostream& output) const{
}

std::unique_ptr<ISheet> CreateSheet() {
    return std::make_unique<Sheet>();
}