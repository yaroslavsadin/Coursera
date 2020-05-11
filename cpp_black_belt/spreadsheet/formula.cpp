#include "common.h"
#include "formula_impl.h"
#include <sstream>
#include <unordered_map>
#include "ast_print_visitor.h"
#include "ast_ref_cells_visitor.h"
#include "ast_insert_visitor.h"
#include "ast_delete_visitor.h"
#include "profile_advanced.h"

static const std::unordered_map<FormulaError::Category,std::string> formula_err_to_string 
{
    {FormulaError::Category::Ref, "#REF!"},
    {FormulaError::Category::Value, "#VALUE!"},
    {FormulaError::Category::Div0, "#DIV/0!"}
};

std::ostream& operator<<(std::ostream& output, FormulaError fe) {
  return output << fe.ToString();
}

FormulaError::FormulaError(Category category) 
: category_(category) {}
FormulaError::Category FormulaError::GetCategory() const {
    return category_;
}
bool FormulaError::operator==(FormulaError rhs) const {
    return this->category_ == rhs.category_;
}
std::string_view FormulaError::ToString() const {
    return formula_err_to_string.at(category_);
}

Formula::Formula(std::string text)
: top(Ast::ParseFormula(text))
{
}

TotalDuration evaluate_duration("evaluate_duration");
Formula::Value Formula::Evaluate(const ISheet& sheet) const{
    ADD_DURATION(evaluate_duration);
    try {    
        return top->Evaluate(sheet);
    } catch(FormulaError err) {
        return err;
    }
}

TotalDuration get_exp_duration("get_exp_duration");
TotalDuration get_refs_duration("get_refs_duration");

std::string Formula::GetExpression() const{
    ADD_DURATION(get_exp_duration);
    Ast::PrintExpressionVisitor visitor;
    top->Accept(visitor);
    return visitor.Get();
}
std::vector<Position> Formula::GetReferencedCells() const{
    ADD_DURATION(get_refs_duration);
    Ast::ReferencedCellsVisitor visitor;
    top->Accept(visitor);
    auto res = visitor.Get();
    for(Position pos : res) {
        if(!pos.IsValid()) {
            throw FormulaException("Referenced cell invalid position");
        }
    }
    return res;
}
Formula::HandlingResult Formula::HandleInsertedRows(int before, int count){
    Ast::InsertVisitor visitor(Ast::InsertVisitor::Type::ROWS, before, count);
    top->Accept(visitor);
    return visitor.Get();
}
Formula::HandlingResult Formula::HandleInsertedCols(int before, int count){
    Ast::InsertVisitor visitor(Ast::InsertVisitor::Type::COLS, before, count);
    top->Accept(visitor);
    return visitor.Get();
}
Formula::HandlingResult Formula::HandleDeletedRows(int first, int count){
    Ast::DeleteVisitor visitor(Ast::DeleteVisitor::Type::ROWS, first, count);
    top->Accept(visitor);
    return visitor.Get();
}
Formula::HandlingResult Formula::HandleDeletedCols(int first, int count){
    Ast::DeleteVisitor visitor(Ast::DeleteVisitor::Type::COLS, first, count);
    top->Accept(visitor);
    return visitor.Get();
}

TotalDuration parse_formula_duration("parse_formula_duration");

std::unique_ptr<IFormula> ParseFormula(std::string expression) {
    ADD_DURATION(parse_formula_duration);
    return std::make_unique<Formula>(std::move(expression));
}