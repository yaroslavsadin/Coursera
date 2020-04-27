#pragma once
#include "formula.h"

class Formula : public IFormula {
public:
    Formula(std::string text);
    virtual Value Evaluate(const ISheet& sheet) const;
    virtual std::string GetExpression() const;
    virtual std::vector<Position> GetReferencedCells() const;
    virtual HandlingResult HandleInsertedRows(int before, int count = 1);
    virtual HandlingResult HandleInsertedCols(int before, int count = 1);
    virtual HandlingResult HandleDeletedRows(int first, int count = 1);
    virtual HandlingResult HandleDeletedCols(int first, int count = 1);
private:
    
};