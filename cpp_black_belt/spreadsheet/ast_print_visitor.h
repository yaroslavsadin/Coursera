#pragma once
#include "ast_node.h"
#include "ast_visitor.h"
#include <sstream>

namespace Ast {
    class AstPrintExpressionVisitor : public NodeVisitor {
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
        static inline bool CheckPlusMinusPresedence(Context ctx) {
            return ctx == Context::UNARY_MINUS || ctx == Context::UNARY_PLUS || 
                    ctx == Context::BINARY_MUL || ctx == Context::BINARY_DIV;
        }
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
        Context current_ctx = Context::MAIN;
        bool is_divisor = false;
        std::stringstream accumulator;
    };
}