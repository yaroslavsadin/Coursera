#pragma once
#include "ast_node.h"
#include "ast_visitor.h"
#include <sstream>

namespace Ast {
    class PrintExpressionVisitor : public NodeVisitor {
    private:
        enum class Context {
            MAIN,
            BINARY_MUL,
            BINARY_DIV,
            BINARY_PLUS,
            BINARY_MINUS,
            UNARY_MINUS,
            UNARY_PLUS
        };
        enum class BinarySide {
            NONE, LHS, RHS
        };
    public:
        virtual void Visit(const NumberNode& node) override;
        virtual void Visit(const UnaryNode<'-'>& node) override;
        virtual void Visit(const UnaryNode<'+'>& node) override;
        virtual void Visit(const BinaryNode<'+'>& node) override;
        virtual void Visit(const BinaryNode<'-'>& node) override;
        virtual void Visit(const BinaryNode<'*'>& node) override;
        virtual void Visit(const BinaryNode<'/'>& node) override;
        virtual void Visit(const CellNode& node) override;
        std::string Get() const;
    private:
        Context ctx = Context::MAIN;
        BinarySide side = BinarySide::NONE;
        std::stringstream accumulator;
    };
}