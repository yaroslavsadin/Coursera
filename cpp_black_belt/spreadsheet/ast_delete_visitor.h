#pragma once
#include "ast_node.h"
#include "ast_visitor.h"
#include "formula.h"
#include <unordered_set>

namespace Ast {
    class DeleteVisitor : public NodeVisitorModifier {
    public:
        enum class Type { ROWS, COLS};
        DeleteVisitor(Type type, int first, int count)
        : type(type), first(first), count(count), res(IFormula::HandlingResult::NothingChanged) {}

        virtual void Visit(CellNode& node) override {
            auto& pos = node.GetPosition();
            auto& val = (type == Type::ROWS) ? pos.row : pos.col;
            if(val >= first && val < first + count) {
                pos = CellNode::REF;
                res = IFormula::HandlingResult::ReferencesChanged;
            } else if(val >= first + count) {
                val -= count;
                if(res == IFormula::HandlingResult::NothingChanged) {
                    res = IFormula::HandlingResult::ReferencesRenamedOnly;
                }
            }
        }
        IFormula::HandlingResult Get() {
            return res;
        }
    private:
        Type type;
        int first; 
        int count;
        IFormula::HandlingResult res;
    };
}