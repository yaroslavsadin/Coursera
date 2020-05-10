#include "cell.h"

Cell::Cell(const ISheet& sheet, std::string str, std::list<std::weak_ptr<Cell>> subscribers) 
: sheet(sheet), subscribers(subscribers) {
    if(str[0] == '=' && str.size() > 1) {
        std::string_view view(str);
        view.remove_prefix(1);
        formula = ParseFormula(std::string(view));
        referenced_cells = formula->GetReferencedCells();
        text = "=" + formula->GetExpression();
    } else {
        text = str;
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
    return text;
}
std::vector<Position> Cell::GetReferencedCells() const {
    return referenced_cells;
}

void Cell::HandleInsertedRows(int before, int count) {
    if(formula) {
        auto result = formula->HandleInsertedRows(before,count);
        if(result == IFormula::HandlingResult::ReferencesRenamedOnly) {
            text = "=" + formula->GetExpression();
        }
        if(result == IFormula::HandlingResult::ReferencesChanged) {
            referenced_cells = formula->GetReferencedCells();
            text = "=" + formula->GetExpression();
        }
    }
}

void Cell::HandleInsertedCols(int before, int count) {
    if(formula) {
        auto result = formula->HandleInsertedCols(before,count);
        if(result == IFormula::HandlingResult::ReferencesRenamedOnly) {
            text = "=" + formula->GetExpression();
        }
        if(result == IFormula::HandlingResult::ReferencesChanged) {
            referenced_cells = formula->GetReferencedCells();
            text = "=" + formula->GetExpression();
        }
    }
}

void Cell::HandleDeletedRows(int first, int count) {
    if(formula) {
        auto result = formula->HandleDeletedRows(first,count);
        if(result == IFormula::HandlingResult::ReferencesRenamedOnly) {
            text = "=" + formula->GetExpression();
        }
        if(result == IFormula::HandlingResult::ReferencesChanged) {
            referenced_cells = formula->GetReferencedCells();
            text = "=" + formula->GetExpression();
        }
    }
}
void Cell::HandleDeletedCols(int first, int count) {
    if(formula) {
        auto result = formula->HandleDeletedCols(first,count);
        if(result == IFormula::HandlingResult::ReferencesRenamedOnly) {
            text = "=" + formula->GetExpression();
        }
        if(result == IFormula::HandlingResult::ReferencesChanged) {
            referenced_cells = formula->GetReferencedCells();
            text = "=" + formula->GetExpression();
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

void Cell::Subscribe(std::weak_ptr<Cell> observer) const {
    subscribers.push_back(observer);
}
void Cell::Notify() const {
    for(auto observer : subscribers) {
        /// TODO: Deal with dangling pointers
        auto ptr = observer.lock();
        if(ptr) ptr->Update();
    }
}

void Cell::Update() const {
    if(cache.HasValue()) {
        cache.Invalidate();
        Notify();
    }
}

Cell::~Cell() {
    Notify();
}

std::shared_ptr<Cell> MakeCell(const ISheet& sheet, std::string str, Position pos) {
    std::list<std::weak_ptr<Cell>> subscribers;
    if(sheet.GetCell(pos)) {
        subscribers = (*static_cast<const Cell*>(sheet.GetCell(pos))).GetSubscribers();
    }
    auto ptr = std::make_shared<Cell>(sheet,std::move(str),subscribers);
    ptr->CheckCircular(pos);
    return ptr;
}