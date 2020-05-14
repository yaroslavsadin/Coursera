#include "cell.h"
#include <cassert>
#include "profile_advanced.h"

TotalDuration cell_ctor_duration("cell_ctor_duration");

Cell::Cell(const ISheet& sheet, Position pos, std::string str, std::unordered_set<const Cell*> subscribers) 
: sheet(sheet), pos(pos), subscribers(subscribers) {
    ADD_DURATION(cell_ctor_duration);
    if(str[0] == '=' && str.size() > 1) {
        std::string_view view(str);
        view.remove_prefix(1);
        formula = ParseFormula(std::string(view));
        referenced_cells = formula->GetReferencedCells();
        text = "=" + formula->GetExpression();
        Cell::GetValue();
    } else {
        text = str;
        std::string_view view = str;
        if(view[0] == '\'') {
            view.remove_prefix(1);
        }
        cache.SetValue(std::string(view));
    }
    for(auto* cell : subscribers) {
        cell->subscriptions.insert(this);
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

void Cell::Subscribe(const Cell* subscriber) const {
    subscribers.insert(subscriber);
    subscriber->subscriptions.insert(this);
}
void Cell::Unsubscribe(const Cell* subscriber) const {   
    subscribers.erase(subscriber);
}
void Cell::Notify() const {
    for(const auto* subscriber : subscribers) {
        subscriber->Update();
    }
}

void Cell::Update() const {
    if(cache.HasValue()) {
        cache.Invalidate();
        Notify();
    }
}

Cell::~Cell() {
    for(auto* cell : subscriptions) {
        cell->subscribers.erase(this);
    }
    for(auto* cell : subscribers) {
        cell->subscriptions.erase(this);
    }
    Notify();
}

TotalDuration make_cell_duration("make_cell_duration");

std::unique_ptr<Cell> MakeCell(const ISheet& sheet, std::string str, Position pos) {
    ADD_DURATION(make_cell_duration);
    std::unordered_set<const Cell*> subscribers;
    if(sheet.GetCell(pos)) {
        subscribers = (*static_cast<const Cell*>(sheet.GetCell(pos))).GetSubscribers();
    }
    auto ptr = std::make_unique<Cell>(sheet,pos,std::move(str),subscribers);
    return ptr;
}