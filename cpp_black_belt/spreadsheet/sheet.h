#pragma once
#include "common.h"
#include "table.h"

class Cell;

class Sheet : public ISheet {
public:
    virtual void SetCell(Position pos, std::string text) override;
    virtual const ICell* GetCell(Position pos) const override;
    virtual ICell* GetCell(Position pos) override;
    virtual void ClearCell(Position pos) override;
    virtual void InsertRows(int before, int count = 1) override;
    virtual void InsertCols(int before, int count = 1) override;
    virtual void DeleteRows(int first, int count = 1) override;
    virtual void DeleteCols(int first, int count = 1) override;
    virtual Size GetPrintableSize() const override;
    virtual void PrintValues(std::ostream& output) const override;
    virtual void PrintTexts(std::ostream& output) const override;
private:
    Table<std::shared_ptr<Cell>> storage;
};
