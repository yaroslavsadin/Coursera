#pragma once
#include "common.h"
#include "formula.h"

class Cell : public ICell {
public:
    Cell(const ISheet& sheet, std::string str);
    virtual Value GetValue() const override;
    virtual std::string GetText() const override;
    virtual std::vector<Position> GetReferencedCells() const override;
private:
    const ISheet& sheet;
    Value value;
    std::unique_ptr<IFormula> formula;
};