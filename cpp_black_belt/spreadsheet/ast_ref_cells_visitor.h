#pragma once
#include "ast_node.h"
#include "ast_visitor.h"
#include <unordered_set>

namespace Ast {
    class ReferencedCellsVisitor : public NodeVisitor {
    public:
        virtual void Visit(const CellNode& node) override {
            if(!counted.count(node.GetPosition().ToString()) && !(node.GetPosition() == CellNode::REF)) {
                referenced_cells.push_back(node.GetPosition());
                counted.insert(node.GetPosition().ToString());
            }
        }
        std::vector<Position> Get() {
            return std::move(referenced_cells);
        }
    private:
        std::unordered_set<std::string> counted;
        std::vector<Position> referenced_cells;
    };
}