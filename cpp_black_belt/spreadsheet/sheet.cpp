#include <algorithm>
#include <cassert>
#include <sstream>
#include "sheet.h"
#include "cell.h"
#include "profile_advanced.h"

bool Size::operator==(const Size& rhs) const {
    return rows == rhs.rows && cols == rhs.cols;
}

static inline void CheckPosition(Position pos, std::string message) {
    if(!pos.IsValid()) {
        throw InvalidPositionException(message);
    }
}

void CircularDependencyChecker::Check() {
    if(sheet.GetCell(pos) && static_cast<const Cell*>(sheet.GetCell(pos))->GetSubscribers().count(cell)) {
        throw CircularDependencyException("CircularDependencyException");
    } else if(cell->GetSubscribers().empty())  {
        return;
    } else {
        CheckRecursevly(cell);
    }
}

void CircularDependencyChecker::CheckRecursevly(const Cell* current) {
    for(const Cell* next : current->GetSubscribers()) {
        if(!checked.count(next)) {
            if(next == cell) {
                throw CircularDependencyException("CircularDependencyException");
            }
            CheckRecursevly(next);
            checked.insert(next);
        }
    }
}

void Sheet::SetCell(Position pos, std::string text){
    
    if(storage.GetCell(pos) && storage.GetCell(pos)->GetText() == text) {
        return;
    }
    CheckPosition(pos, __FUNCTION__);
    auto cell = MakeCell(*this,text,pos);
    for(auto referenced : cell->GetReferencedCells()) {
        if(storage.GetCell(referenced) == nullptr) {
            storage.SetCell(referenced,MakeCell(*this,"",referenced));
        }
        storage.GetCell(referenced)->Subscribe(cell.get());
    }
    CircularDependencyChecker(pos, cell.get(), *this).Check();
    storage.SetCell(pos,std::move(cell));
}
const ICell* Sheet::GetCell(Position pos) const{ 
    CheckPosition(pos, __FUNCTION__);
    const auto* cell = storage.GetCell(pos);
    if(cell) {
        return static_cast<const ICell*>(cell);
    } else {
        return nullptr;
    }
}
ICell* Sheet::GetCell(Position pos){
    CheckPosition(pos, __FUNCTION__);
    auto* cell = storage.GetCell(pos);
    if(cell) {
        return static_cast<ICell*>(cell);
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
        for(auto& row : storage.Get()) {
            for(auto& cell : row) {
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
        for(auto& row : storage.Get()) {
            for(auto& cell : row) {
                if(cell) {    
                    cell->HandleInsertedCols(before,count);
                }
            }
        }
    }
}

void Sheet::DeleteRows(int first, int count){
    storage.DeleteRows(first,count);
    for(auto& row : storage.Get()) {
        for(auto& cell : row) {
            if(cell) {
                cell->HandleDeletedRows(first,count);
            }
        }
    }
}
void Sheet::DeleteCols(int first, int count){
    storage.DeleteCols(first,count);
    for(auto& row : storage.Get()) {
        for(auto& cell : row) {
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
    std::string res;
    for(const auto& row : storage.Get()) {
        for(size_t i = 0; i < storage.GetColCount(); i++) {
            if(i < row.size()) {
                if(auto& cell = row[i]; cell) {
                    std::stringstream ss;
                    std::visit(
                    [&ss](const auto& val) {
                        ss << val;
                        return ss.str();
                    }, 
                    cell->GetValue());
                    res += ss.str();
                }
            }
            res.push_back('\t');
        }
        res.back() = '\n';
    }
    output << res;
}
void Sheet::PrintTexts(std::ostream& output) const{
    std::string res;
    for(const auto& row : storage.Get()) {
        for(size_t i = 0; i < storage.GetColCount(); i++) {
            if(i < row.size()) {
                if(auto& cell = row[i]; cell) {
                    res += cell->GetText();
                }
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