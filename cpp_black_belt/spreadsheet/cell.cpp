#include "cell.h"

Cell::Cell(const ISheet& sheet, std::string str) : sheet(sheet) {
    if(str[0] == '=' && str.size() > 1) {
        std::string_view view(str);
        view.remove_prefix(1);
        formula = ParseFormula(std::string(view));
    } else {
        cache.SetText(str);
        std::string_view view = str;
        if(view[0] == '\'') {
            view.remove_prefix(1);
        }
        cache.SetValue(std::string(view));
    }
}

Cell::Value Cell::GetValue() const {
    if(!cache.HasValue()) {
        // Imply we have formula
        auto res = formula->Evaluate(sheet);
        if(std::holds_alternative<double>(res)) {
            cache.SetValue(std::get<double>(res));
        } else {
            cache.SetValue(std::get<FormulaError>(res));
        }
    }
    return cache.GetValue();
}
std::string Cell::GetText() const {
    if(!cache.HasText()) {
        cache.SetText("=" + formula->GetExpression());
    }
    return cache.GetText();
}
std::vector<Position> Cell::GetReferencedCells() const {
    if(formula) {
        return formula->GetReferencedCells();
    } else {    
        return {};
    }
}

void Cell::HandleInsertedRows(int before, int count) {
    if(formula) {
        auto result = formula->HandleInsertedRows(before,count);
        if(result == IFormula::HandlingResult::ReferencesChanged) {
            Notify();
        }
    }
}

void Cell::HandleInsertedCols(int before, int count) {
    if(formula) {
        auto result = formula->HandleInsertedCols(before,count); 
        if(result == IFormula::HandlingResult::ReferencesChanged) {
            Notify();
        }
    }
}

void Cell::HandleDeletedRows(int first, int count) {
    if(formula) {
        auto result = formula->HandleDeletedRows(first,count);
        if(result == IFormula::HandlingResult::ReferencesChanged) {
            Notify();
        }
    }
}
void Cell::HandleDeletedCols(int first, int count) {
    if(formula) {
        auto result = formula->HandleDeletedCols(first,count);
        if(result == IFormula::HandlingResult::ReferencesChanged) {
            Notify();
        }
    }
}

void Cell::CheckCircular(Position self) const {
    for(auto pos : GetReferencedCells()) {
        if(pos == self) {
            throw CircularDependencyException("");
        } else {
            auto cell_ptr = sheet.GetCell(pos);
            if(cell_ptr) {
                static_cast<const Cell*>(cell_ptr)->CheckCircular(self);
            }
        }
    }
}