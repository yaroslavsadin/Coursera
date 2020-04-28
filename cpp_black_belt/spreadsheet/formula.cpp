#include "common.h"
#include "formula_impl.h"

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
: text(std::move(text)), top(Ast::ParseFormula(this->text))
{
}
Formula::Value Formula::Evaluate(const ISheet& sheet) const{
    return top->Evaluate(sheet);
}
std::string Formula::GetExpression() const{
    return text;
}
std::vector<Position> Formula::GetReferencedCells() const{
}
Formula::HandlingResult Formula::HandleInsertedRows(int before, int count){
}
Formula::HandlingResult Formula::HandleInsertedCols(int before, int count){
}
Formula::HandlingResult Formula::HandleDeletedRows(int first, int count){
}
Formula::HandlingResult Formula::HandleDeletedCols(int first, int count){
}

std::unique_ptr<IFormula> ParseFormula(std::string expression) {
    return std::make_unique<Formula>(std::move(expression));
}