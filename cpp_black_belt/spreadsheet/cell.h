#pragma once
#include "common.h"

class Cell : public ICell {
public:
    Cell(std::string str);
    virtual Value GetValue() const override;
    virtual std::string GetText() const override;
    virtual std::vector<Position> GetReferencedCells() const override;
private:
    Value value;
    std::string text;
    std::vector<Position> referenced;
};