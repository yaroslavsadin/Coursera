#include "ast_print_visitor.h"

namespace Ast {
    void PrintExpressionVisitor::Visit(const NumberNode& node) {
        accumulator << node.GetValue();
    }
    void PrintExpressionVisitor::Visit(const UnaryNode<'-'>& node) {
        current_ctx = Context::UNARY_MINUS;
        accumulator << '-';
        node.GetRight().Accept(*this);
    }
    void PrintExpressionVisitor::Visit(const UnaryNode<'+'>& node){
        current_ctx = Context::UNARY_PLUS;
        accumulator << '+';
        node.GetRight().Accept(*this);
    }
    void PrintExpressionVisitor::Visit(const BinaryNode<'+'>& node){
        auto ctx = current_ctx;
        current_ctx = Context::BINARY_PLUS;
        if(CheckPlusMinusPresedence(ctx)) {
            accumulator << '(';
        }
        node.GetLeft().Accept(*this);
        accumulator << '+';
        node.GetRight().Accept(*this);
        if(CheckPlusMinusPresedence(ctx)) {
            accumulator << ')';
        }
    }
    void PrintExpressionVisitor::Visit(const BinaryNode<'-'>& node){
        auto ctx = current_ctx;
        current_ctx = Context::BINARY_MINUS;
        if(CheckPlusMinusPresedence(ctx)) {
            accumulator << '(';
        }
        node.GetLeft().Accept(*this);
        accumulator << '-';
        node.GetRight().Accept(*this);
        if(CheckPlusMinusPresedence(ctx)) {
            accumulator << ')';
        }
    }
    void PrintExpressionVisitor::Visit(const BinaryNode<'*'>& node){
        current_ctx = Context::BINARY_MUL;
        node.GetLeft().Accept(*this);
        accumulator << '*';
        node.GetRight().Accept(*this);
    }
    void PrintExpressionVisitor::Visit(const BinaryNode<'/'>& node){
        auto ctx = current_ctx;
        bool is_divisor_ = is_divisor;
        current_ctx = Context::BINARY_DIV;
        is_divisor = false;
        if(ctx == Context::BINARY_DIV && is_divisor_) {
            accumulator << '(';
        }
        node.GetLeft().Accept(*this);
        accumulator << '/';
        is_divisor = true;
        node.GetRight().Accept(*this);
        is_divisor = false;
        if(ctx == Context::BINARY_DIV && is_divisor_) {
            accumulator << ')';
        }
    }
    void PrintExpressionVisitor::Visit(const CellNode& node){
        accumulator << node.GetPosition().ToString();
    }
    std::string PrintExpressionVisitor::Get() const {
        return accumulator.str();
    }
}