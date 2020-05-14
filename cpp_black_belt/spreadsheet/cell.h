#pragma once
#include <iostream>
#include <optional>
#include <unordered_set>
#include "common.h"
#include "formula.h"

class Cell : public ICell {
public:
    Cell(const ISheet& sheet, std::string str, std::unordered_set<const Cell*> subscribers);
    virtual Value GetValue() const override;
    virtual std::string GetText() const noexcept override;
    virtual std::vector<Position> GetReferencedCells() const noexcept override;
    void HandleInsertedRows(int before, int count = 1);
    void HandleInsertedCols(int before, int count = 1);
    void HandleDeletedRows(int first, int count = 1);
    void HandleDeletedCols(int first, int count = 1);
    
    // Observer part
    void Subscribe(const Cell* observer) const;
    void Notify() const;
    const std::unordered_set<const Cell*>& GetSubscribers() const {
        return subscribers;
    }
    const std::unordered_set<const Cell*>& GetSubscritions() const {
        return subscriptions;
    }

    // Subscriber part
    void Update() const;

    virtual ~Cell();
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
        void Invalidate() {
            value.reset();
        }
    private:
        std::optional<Value> value;
    };

    const ISheet& sheet;
    std::unique_ptr<IFormula> formula;
    std::string text;
    std::vector<Position> referenced_cells;
    mutable CellCache cache;
    mutable std::unordered_set<const Cell*> subscriptions;
    mutable std::unordered_set<const Cell*> subscribers;
};

std::unique_ptr<Cell> MakeCell(const ISheet& sheet, std::string str, Position pos);