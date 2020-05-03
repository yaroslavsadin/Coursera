#pragma once
#include "common.h"

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
    size_t NumRows() const { return storage.size(); }
    size_t NumCols() const { return (storage.empty()) ? 0u : storage[0].size(); }
    void Extend(size_t rows, size_t cols);
    bool CellExists(Position pos) const;
    std::vector<std::vector<std::unique_ptr<ICell>>> storage;
};
