#pragma once
#include "ast_node.h"
#include "ast_visitor.h"
#include "formula.h"
#include <unordered_set>

namespace Ast {
    class InsertVisitor : public NodeVisitorModifier {
    public:
        enum class Type { ROWS, COLS};
        InsertVisitor(Type type, int before, int count)
        : type(type), before(before), count(count), res(IFormula::HandlingResult::NothingChanged) {}
        virtual void Visit(CellNode& node) override {
            auto pos = node.GetPosition();
            auto& val = (type == Type::ROWS) ? pos.row : pos.col;
            if(val >= before) {
                val += count;
                node.SetPosition(pos);
                res = IFormula::HandlingResult::ReferencesRenamedOnly;
            }
        }
         IFormula::HandlingResult Get() {
            return res;
        }
    private:
        Type type;
        int before; 
        int count;
        IFormula::HandlingResult res;
    };
}