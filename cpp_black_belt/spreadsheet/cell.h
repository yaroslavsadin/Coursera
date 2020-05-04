#pragma once
#include "common.h"
#include "formula.h"

class Cell : public ICell {
public:
    Cell(const ISheet& sheet, std::string str);
    virtual Value GetValue() const override;
    virtual std::string GetText() const override;
    virtual std::vector<Position> GetReferencedCells() const override;
    virtual void HandleInsertedRows(int before, int count = 1) override;
    virtual void HandleInsertedCols(int before, int count = 1) override;
    virtual void HandleDeletedRows(int first, int count = 1) override;
    virtual void HandleDeletedCols(int first, int count = 1) override;
private:
    const ISheet& sheet;
    Value value;
    std::unique_ptr<IFormula> formula;
};