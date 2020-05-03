#include "cell.h"

Cell::Cell(const ISheet& sheet, std::string str) : sheet(sheet) {
    if(str[0] == '=' && str.size() > 1) {
        std::string_view view(str);
        view.remove_prefix(1);
        formula = ParseFormula(std::string(view));
    } else {
        value = str;
    }
}

Cell::Value Cell::GetValue() const {
    std::string_view view = std::get<std::string>(value);
    if(view[0] == '\'') {
        view.remove_prefix(1);
    }
    return std::string(view);
}
std::string Cell::GetText() const {
    if(formula) {
        return "=" + formula->GetExpression();
    } else {
        return std::get<std::string>(value);
    }
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
        auto result = formula->HandleDeletedRows(before,count);
        /// TODO: do something with the result
    }
}

void Cell::HandleInsertedCols(int before, int count) {
    if(formula) {
        auto result = formula->HandleDeletedCols(before,count);
        /// TODO: do something with the result
    }
}