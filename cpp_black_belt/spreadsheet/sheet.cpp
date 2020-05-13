#include <algorithm>
#include <cassert>
#include <sstream>
#include "sheet.h"
#include "cell.h"
#include "profile_advanced.h"

TotalDuration set_cell_duration("set_cell_duration");
TotalDuration clear_cell_duration("clear_cell_duration");
TotalDuration delete_cols_duration("delete_cols_duration");
TotalDuration delete_rows_duration("delete_rows_duration");
TotalDuration get_cell_duration("get_cell_duration");
TotalDuration set_printable_duration("set_printable_duration");
TotalDuration insert_cols_duration("insert_cols_duration");
TotalDuration insert_rows_duration("insert_rows_duration");
TotalDuration print_texts_duration("print_texts_duration");
TotalDuration print_values_duration("print_values_duration");
TotalDuration circular_duration("circular_duration");

extern TotalDuration make_cell_duration;
extern TotalDuration parse_formula_duration;
extern TotalDuration get_exp_duration;
extern TotalDuration get_refs_duration;
extern TotalDuration cell_ctor_duration;
extern TotalDuration evaluate_duration;

bool Size::operator==(const Size& rhs) const {
    return rows == rhs.rows && cols == rhs.cols;
}

static inline void CheckPosition(Position pos, std::string message) {
    if(!pos.IsValid()) {
        throw InvalidPositionException(message);
    }
}

static size_t count____ = 0;
static std::vector<Position> poss___;
static std::vector<std::string> texts___;
void Sheet::SetCell(Position pos, std::string text){
    if(count____ >= 1300 && count____ < 1500) {
        poss___.push_back(pos);
        texts___.push_back(text);
    }
    if(++count____ > 10000) {
        std::stringstream ss;
        ss << "SetCell: " << duration_cast<milliseconds>(set_cell_duration.value).count() << std::endl <<
        "GetCell: " << duration_cast<milliseconds>(get_cell_duration.value).count() << std::endl <<
        "ClearCell: " << duration_cast<milliseconds>(clear_cell_duration.value).count() << std::endl <<
        "DeleteCols: " << duration_cast<milliseconds>(delete_cols_duration.value).count() << std::endl <<
        "DeleteRows: " << duration_cast<milliseconds>(delete_rows_duration.value).count() << std::endl <<
        "GetPrintable: " << duration_cast<milliseconds>(set_printable_duration.value).count() << std::endl <<
        "InsertRows: " << duration_cast<milliseconds>(insert_rows_duration.value).count() << std::endl <<
        "InsertCols: " << duration_cast<milliseconds>(insert_cols_duration.value).count() << std::endl <<
        "PrintTexts: " << duration_cast<milliseconds>(print_texts_duration.value).count() << std::endl <<
        "PrintValues: " << duration_cast<milliseconds>(print_values_duration.value).count() << std::endl <<
        "MakeCell: " << duration_cast<milliseconds>(make_cell_duration.value).count() << std::endl <<
        "Formula::GetReferencedCells: " << duration_cast<milliseconds>(get_refs_duration.value).count() << std::endl <<
        "Formula::GetExpression: " << duration_cast<milliseconds>(get_exp_duration.value).count() << std::endl <<
        "Formula::Evaluate: " << duration_cast<milliseconds>(evaluate_duration.value).count() << std::endl <<
        "Cell::Cell: " << duration_cast<milliseconds>(cell_ctor_duration.value).count() << std::endl <<
        "Cell::CheckCircular: " << duration_cast<milliseconds>(circular_duration.value).count() << std::endl <<
        "ParseFormula: " << duration_cast<milliseconds>(parse_formula_duration.value).count() << std::endl;

        for(size_t i = 0; i < 200; i++) {
            ss << "{" << poss___[i].row << "," << poss___[i].col << "} " << texts___[i] << '\n';
        }
        ss << std::endl;

        throw std::runtime_error(ss.str());
    }
    ADD_DURATION(set_cell_duration);
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
    {
        ADD_DURATION(circular_duration);
        if(storage.GetCell(pos)) {
            cell->CheckCircular(cell.get(),storage.GetCell(pos));
        } else {
            cell->CheckCircular(cell.get(),cell.get());
        }
    }
    storage.SetCell(pos,std::move(cell));
}
const ICell* Sheet::GetCell(Position pos) const{
    ADD_DURATION(get_cell_duration);
    CheckPosition(pos, __FUNCTION__);
    if(storage.GetCell(pos)) {
        return static_cast<const ICell*>(storage.GetCell(pos));
    } else {
        return nullptr;
    }
}
ICell* Sheet::GetCell(Position pos){
    ADD_DURATION(get_cell_duration);
    CheckPosition(pos, __FUNCTION__);
    if(storage.GetCell(pos)) {
        return static_cast<ICell*>(storage.GetCell(pos));
    } else {
        return nullptr;
    }
}
void Sheet::ClearCell(Position pos){
    ADD_DURATION(clear_cell_duration);
    CheckPosition(pos, __FUNCTION__);
    storage.ClearCell(pos);
}
void Sheet::InsertRows(int before, int count){
    ADD_DURATION(insert_rows_duration);
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
    ADD_DURATION(insert_cols_duration);
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
    ADD_DURATION(delete_rows_duration);
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
    ADD_DURATION(delete_cols_duration);
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
    ADD_DURATION(set_printable_duration);
    return {storage.GetRowCount(),storage.GetColCount()};
}
void Sheet::PrintValues(std::ostream& output) const{
    ADD_DURATION(print_values_duration);
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
    ADD_DURATION(print_texts_duration);
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