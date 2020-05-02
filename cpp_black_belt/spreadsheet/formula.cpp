#include "common.h"
#include "formula_impl.h"
#include <sstream>
#include "ast_print_visitor.h"
#include "ast_ref_cells_visitor.h"
#include "ast_insert_visitor.h"

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
Formula::Value Formula::Evaluate(const ISheet& sheet) const{
    return top->Evaluate(sheet);
}
std::string Formula::GetExpression() const{
    Ast::PrintExpressionVisitor visitor;
    top->Accept(visitor);
    return visitor.Get();
}
std::vector<Position> Formula::GetReferencedCells() const{
    Ast::ReferencedCellsVisitor visitor;
    top->Accept(visitor);
    return visitor.Get();
}
Formula::HandlingResult Formula::HandleInsertedRows(int before, int count){
    Ast::InsertVisitor visitor(Ast::InsertVisitor::Type::ROWS, before, count);
    top->Accept(visitor);
    return (visitor.Get()) ? HandlingResult::ReferencesRenamedOnly : HandlingResult::NothingChanged;
}
Formula::HandlingResult Formula::HandleInsertedCols(int before, int count){
    Ast::InsertVisitor visitor(Ast::InsertVisitor::Type::COLS, before, count);
    top->Accept(visitor);
    return (visitor.Get()) ? HandlingResult::ReferencesRenamedOnly : HandlingResult::NothingChanged;
}
Formula::HandlingResult Formula::HandleDeletedRows(int first, int count){
}
Formula::HandlingResult Formula::HandleDeletedCols(int first, int count){
}

std::unique_ptr<IFormula> ParseFormula(std::string expression) {
    return std::make_unique<Formula>(std::move(expression));
}