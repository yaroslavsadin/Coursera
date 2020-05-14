#pragma once
#include "common.h"
#include "table.h"
#include <unordered_set>

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
    virtual ~Sheet() = default;
private:
    Table<Cell> storage;
};

class CircularDependencyChecker {
public:
    CircularDependencyChecker(const Sheet& sheet, Position pos, const Cell* cell) 
    : pos(pos), cell(cell), sheet(sheet) {}

    void Check();
    void CheckRecursevly(const Cell* current);
private:
    Position pos;
    const Cell* cell;
    const Sheet& sheet;
    std::unordered_set<const Cell*> checked;
};