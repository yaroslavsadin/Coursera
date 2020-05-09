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
    storage.SetCell(pos.row,pos.col,item);
    for(auto referenced : storage.GetCell(pos.row,pos.col)->GetReferencedCells()) {
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
    if(static_cast<size_t>(before) < storage.GetRowCount() && storage.GetRowCount() + count > Position::kMaxRows) {
        throw TableTooBigException(__FUNCTION__);
    } else {
        storage.InsertRows(before,count);
        for(const auto [pos,cell] : storage) {
            cell->HandleInsertedRows(before,count);
        }
    }
}
void Sheet::InsertCols(int before, int count){    
    if(static_cast<size_t>(before) < storage.GetColCount() && storage.GetColCount() + count > Position::kMaxCols) {
        throw TableTooBigException(__FUNCTION__);
    } else {
        storage.InsertCols(before,count);
        for(const auto [pos,cell] : storage) {
            cell->HandleInsertedCols(before,count);
        }
    }
}

void Sheet::DeleteRows(int first, int count){
    storage.DeleteRows(static_cast<size_t>(first),count);
    for(const auto [pos,cell] : storage) {
        cell->HandleDeletedRows(first,count);
    }
}
void Sheet::DeleteCols(int first, int count){
    storage.DeleteCols(static_cast<size_t>(first),count);
    for(const auto [pos,cell] : storage) {
        cell->HandleDeletedCols(first,count);
    }
}
Size Sheet::GetPrintableSize() const{
    return {static_cast<int>(storage.GetRowCount()),static_cast<int>(storage.GetColCount())};
}
void Sheet::PrintValues(std::ostream& output) const{
    // std::string res;
    // for(const auto [pos,cell] : storage) {
    //     for(auto i = 0u; i < storage.GetColCount(); i++) {
    //         if(i < row.size() && row[i] != nullptr) {
    //             std::stringstream ss;
    //             std::visit(
    //             [&ss](const auto& val) {
    //                 ss << val;
    //                 return ss.str();
    //             }, 
    //             row[i]->GetValue());
    //             res += ss.str();
    //         }
    //         res.push_back('\t');
    //     }
    //     res.back() = '\n';
    // }
    // output << res;
}
void Sheet::PrintTexts(std::ostream& output) const{
    // std::string res;
    // for(const auto& row : storage) {
    //     for(auto i = 0u; i < storage.GetColCount(); i++) {
    //         if(i < row.size() && row[i] != nullptr) {
    //             res += row[i]->GetText();
    //         }
    //         res.push_back('\t');
    //     }
    //     res.back() = '\n';
    // }
    // output << res;
}

std::unique_ptr<ISheet> CreateSheet() {
    return std::make_unique<Sheet>();
}