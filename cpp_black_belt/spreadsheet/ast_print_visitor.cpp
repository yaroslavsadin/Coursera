#include "ast_print_visitor.h"

namespace Ast {
    void PrintExpressionVisitor::Visit(const NumberNode& node) {
        accumulator << node.GetValue();
    }
    void PrintExpressionVisitor::Visit(const UnaryNode<'-'>& node) {
        ctx = Context::UNARY_MINUS;
        accumulator << '-';
        node.GetRight().Accept(*this);
    }
    void PrintExpressionVisitor::Visit(const UnaryNode<'+'>& node){
        ctx = Context::UNARY_PLUS;
        accumulator << '+';
        node.GetRight().Accept(*this);
    }
    void PrintExpressionVisitor::Visit(const BinaryNode<'+'>& node){
        auto parent_ctx = ctx;
        auto current_side = side;

        auto parens_needed_f = [parent_ctx,current_side]() {
            return (parent_ctx == Context::BINARY_MINUS && current_side == BinarySide::RHS) ||
                    parent_ctx == Context::BINARY_MUL || parent_ctx == Context::BINARY_DIV || 
                    parent_ctx == Context::UNARY_MINUS || parent_ctx == Context::UNARY_PLUS;
        };

        if(parens_needed_f()) {
            accumulator << '(';
        }

        ctx = Context::BINARY_PLUS;
        side = BinarySide::LHS;
        node.GetLeft().Accept(*this);
        accumulator << '+';
        ctx = Context::BINARY_PLUS;
        side = BinarySide::RHS;
        node.GetRight().Accept(*this);

        if(parens_needed_f()) {
            accumulator << ')';
        }
    }
    void PrintExpressionVisitor::Visit(const BinaryNode<'-'>& node){
        auto parent_ctx = ctx;
        auto current_side = side;

        auto parens_needed_f = [parent_ctx,current_side]() {
            return (parent_ctx == Context::BINARY_MINUS && current_side == BinarySide::RHS) ||
                    parent_ctx == Context::BINARY_MUL || parent_ctx == Context::BINARY_DIV || 
                    parent_ctx == Context::UNARY_MINUS || parent_ctx == Context::UNARY_PLUS;
        };

        if(parens_needed_f()) {
            accumulator << '(';
        }

        ctx = Context::BINARY_MINUS;
        side = BinarySide::LHS;
        node.GetLeft().Accept(*this);
        accumulator << '-';
        ctx = Context::BINARY_MINUS;
        side = BinarySide::RHS;
        node.GetRight().Accept(*this);

        if(parens_needed_f()) {
            accumulator << ')';
        }
    }
    void PrintExpressionVisitor::Visit(const BinaryNode<'*'>& node){
        auto parent_ctx = ctx;
        auto current_side = side;

        auto parens_needed_f = [parent_ctx,current_side]() {
            return (parent_ctx == Context::BINARY_DIV && current_side == BinarySide::RHS);
        };

        if(parens_needed_f()) {
            accumulator << '(';
        }

        ctx = Context::BINARY_MUL;
        side = BinarySide::LHS;
        node.GetLeft().Accept(*this);
        accumulator << '*';
        ctx = Context::BINARY_MUL;
        side = BinarySide::RHS;
        node.GetRight().Accept(*this);

        if(parens_needed_f()) {
            accumulator << ')';
        }
    }
    void PrintExpressionVisitor::Visit(const BinaryNode<'/'>& node){
        auto parent_ctx = ctx;
        auto current_side = side;

        auto parens_needed_f = [parent_ctx,current_side]() {
            return (parent_ctx == Context::BINARY_DIV && current_side == BinarySide::RHS);
        };

        if(parens_needed_f()) {
            accumulator << '(';
        }

        ctx = Context::BINARY_DIV;
        side = BinarySide::LHS;
        node.GetLeft().Accept(*this);
        accumulator << '/';
        ctx = Context::BINARY_DIV;
        side = BinarySide::RHS;
        node.GetRight().Accept(*this);

        if(parens_needed_f()) {
            accumulator << ')';
        }
    }
    void PrintExpressionVisitor::Visit(const CellNode& node){
        auto pos = node.GetPosition();
        if(pos == CellNode::REF) {    
            accumulator << "#REF!";
        } else {
            accumulator << pos.ToString();
        }
    }
    std::string PrintExpressionVisitor::Get() const {
        return accumulator.str();
    }
}