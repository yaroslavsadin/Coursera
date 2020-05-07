#pragma once
#include "common.h"
#include "formula.h"
#include <iostream>

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
    class CellCache {
    public:
        CellCache() = default;
        void SetValue(Value new_value) {
            value = std::move(new_value);
        }
        bool HasValue() const {
            return value.has_value();
        }
        Value GetValue() const {
            return *value;
        }
        void SetText(std::string new_text) {
            text = std::move(new_text);
        }
        bool HasText() const {
            return text.has_value();
        }
        const std::string& GetText() const {
            return *text;
        }
        void InvalidateValue() {
            value.reset();
        }
        void InvalidateText() {
            text.reset();
        }
        void Invalidate() {
            InvalidateValue(); 
            InvalidateText();
        }
    private:
        std::optional<Value> value;
        std::optional<std::string> text;
    };

    const ISheet& sheet;
    mutable CellCache cache;
    std::unique_ptr<IFormula> formula;
};