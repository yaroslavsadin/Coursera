#pragma once
#include "formula.h"
#include "ast_node.h"

class Formula : public IFormula {
public:
    Formula(std::string text);
    virtual Value Evaluate(const ISheet& sheet) const override;
    virtual std::string GetExpression() const override;
    virtual std::vector<Position> GetReferencedCells() const override;
    virtual HandlingResult HandleInsertedRows(int before, int count = 1) override;
    virtual HandlingResult HandleInsertedCols(int before, int count = 1) override;
    virtual HandlingResult HandleDeletedRows(int first, int count = 1) override;
    virtual HandlingResult HandleDeletedCols(int first, int count = 1) override;
private:
    std::unique_ptr<Ast::Node> top;
    std::vector<Position> referenced_cells;
};