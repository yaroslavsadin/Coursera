#include "common.h"
#include "formula_impl.h"



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