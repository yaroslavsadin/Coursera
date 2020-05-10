#include <algorithm>
#include <cassert>
#include <sstream>
#include "sheet.h"
#include "cell.h"


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
    item->CheckCircular(pos); // throws CircularDependencyExeption
    storage.SetCell(pos,item);
    for(auto referenced : (*storage.GetCell(pos))->GetReferencedCells()) {
        if(storage.GetCell(referenced) == nullptr) {
            storage.SetCell(referenced,std::make_shared<Cell>(*this,""));
        }
        (*storage.GetCell(referenced))->Subscribe(item);
    }
}
const ICell* Sheet::GetCell(Position pos) const{
    CheckPosition(pos, __FUNCTION__);
    if(storage.GetCell(pos)) {    
        return static_cast<ICell*>(storage.GetCell(pos)->get());
    } else {
        return nullptr;
    }
}
ICell* Sheet::GetCell(Position pos){
    CheckPosition(pos, __FUNCTION__);
    if(storage.GetCell(pos)) {    
        return static_cast<ICell*>(storage.GetCell(pos)->get());
    } else {
        return nullptr;
    }
}
void Sheet::ClearCell(Position pos){
    CheckPosition(pos, __FUNCTION__);
    storage.ClearCell(pos);
}
void Sheet::InsertRows(int before, int count){
    if(before < storage.GetRowCount() && storage.GetRowCount() + count > Position::kMaxRows) {
        throw TableTooBigException(__FUNCTION__);
    } else {
        storage.InsertRows(before,count);
        for(const auto [pos,cell] : storage) {
            cell->HandleInsertedRows(before,count);
        }
    }
}
void Sheet::InsertCols(int before, int count){    
    if(before < storage.GetColCount() && storage.GetColCount() + count > Position::kMaxCols) {
        throw TableTooBigException(__FUNCTION__);
    } else {
        storage.InsertCols(before,count);
        for(const auto [pos,cell] : storage) {
            cell->HandleInsertedCols(before,count);
        }
    }
}

void Sheet::DeleteRows(int first, int count){
    storage.DeleteRows(first,count);
    for(const auto [pos,cell] : storage) {
        cell->HandleDeletedRows(first,count);
    }
}
void Sheet::DeleteCols(int first, int count){
    storage.DeleteCols(first,count);
    for(const auto [pos,cell] : storage) {
        cell->HandleDeletedCols(first,count);
    }
}
Size Sheet::GetPrintableSize() const{
    return {storage.GetRowCount(),storage.GetColCount()};
}
void Sheet::PrintValues(std::ostream& output) const{
    std::string res;
    for(const auto& row : storage.GetPrintable()) {
        for(const auto* cell : row) {
            if(cell != nullptr) {
                std::stringstream ss;
                std::visit(
                [&ss](const auto& val) {
                    ss << val;
                    return ss.str();
                }, 
                (*cell)->GetValue());
                res += ss.str();
            }
            res.push_back('\t');
        }
        res.back() = '\n';
    }
    output << res;
}
void Sheet::PrintTexts(std::ostream& output) const{
    std::string res;
    for(const auto& row : storage.GetPrintable()) {
        for(const auto* cell : row) {
            if(cell != nullptr) {
                res += (*cell)->GetText();
            }
            res.push_back('\t');
        }
        res.back() = '\n';
    }
    output << res;
}

std::unique_ptr<ISheet> CreateSheet() {
    return std::make_unique<Sheet>();
}