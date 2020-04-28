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

Formula::Formula(std::string text){
}
Formula::Value Formula::Evaluate(const ISheet& sheet) const{
}
std::string Formula::GetExpression() const{
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