#pragma once
#include "ast_node.h"
#include "ast_visitor.h"
#include <unordered_set>

namespace Ast {
    class AstReferencedCellsVisitor : public NodeVisitor {
    public:
        virtual void Visit(const NumberNode& node) noexcept override  {
        }
        virtual void Visit(const UnaryNode<'-'>& node) override {
            node.GetRight().Accept(*this);
        }
        virtual void Visit(const UnaryNode<'+'>& node) override {
            node.GetRight().Accept(*this);
        }
        virtual void Visit(const BinaryNode<'+'>& node) override {
            node.GetLeft().Accept(*this);
            node.GetRight().Accept(*this);
        }
        virtual void Visit(const BinaryNode<'-'>& node) override {
            node.GetLeft().Accept(*this);
            node.GetRight().Accept(*this);
        }
        virtual void Visit(const BinaryNode<'*'>& node) override {
            node.GetLeft().Accept(*this);
            node.GetRight().Accept(*this);
        }
        virtual void Visit(const BinaryNode<'/'>& node) override {
            node.GetLeft().Accept(*this);
            node.GetRight().Accept(*this);
        }
        virtual void Visit(const CellNode& node) override {
            if(!counted.count(node.GetPosition().ToString())) {
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