#pragma once
#include <iostream>
#include "common.h"
#include <optional>
#include <sstream>

namespace Ast {
    class NodeVisitor;

    class Node {
    public:
        virtual double Evaluate(const ISheet& context) const = 0;
        virtual void Accept(NodeVisitor& visitor) const = 0;
    };

    class NumberNode;
    template<char op>
    class UnaryNode;
    template<char op>
    class BinaryNode;
    class CellNode;

    class NodeVisitor {
    public:
        virtual void Visit(const NumberNode& node) = 0;
        virtual void Visit(const UnaryNode<'-'>& node) = 0;
        virtual void Visit(const UnaryNode<'+'>& node) = 0;
        virtual void Visit(const BinaryNode<'+'>& node) = 0;
        virtual void Visit(const BinaryNode<'-'>& node) = 0;
        virtual void Visit(const BinaryNode<'*'>& node) = 0;
        virtual void Visit(const BinaryNode<'/'>& node) = 0;
        virtual void Visit(const CellNode& node) = 0;
    };

    class NumberNode : public Node {
    public:
        NumberNode(double value) : value(value) {}
        virtual double Evaluate(const ISheet& context) const noexcept override  {
            return value;
        }
         virtual void Accept(NodeVisitor& visitor) const override {
             visitor.Visit(*this);
         }
         double GetValue() const noexcept {
             return value;
         }
    private:
        double value;
    };

    template<char op>
    class UnaryNode : public Node {
    public:
        UnaryNode(std::unique_ptr<Node> rhs) : rhs(std::move(rhs)) {}
        virtual double Evaluate(const ISheet& context) const override {
            static_assert(op == '+' || op == '-');
            if constexpr(op == '+') {
                return rhs->Evaluate(context);
            } else if constexpr(op == '-') {
                return - rhs->Evaluate(context);
            }
        }
        const Node& GetRight() const {
            return *rhs;
        }
        virtual void Accept(NodeVisitor& visitor) const override {
            visitor.Visit(*this);
        }
    private:
        std::unique_ptr<Node> rhs;
    };

    template<char op>
    class BinaryNode : public Node {
    public:
        BinaryNode(std::unique_ptr<Node> lhs, std::unique_ptr<Node> rhs) : 
        lhs(std::move(lhs)), rhs(std::move(rhs)) {}
        virtual double Evaluate(const ISheet& context) const override {
            static_assert(op == '+' || op == '-' || op == '/' || op == '*');
            if constexpr(op == '+') {
                return lhs->Evaluate(context) + rhs->Evaluate(context);
            } else if constexpr(op == '-') {
                return lhs->Evaluate(context) - rhs->Evaluate(context);
            } else if constexpr(op == '/') {
                auto rhs_val = rhs->Evaluate(context);
                if(!rhs_val) {
                    throw FormulaError(FormulaError::Category::Div0);
                }
                return lhs->Evaluate(context) / rhs_val;
            } else if constexpr(op == '*') {
                return lhs->Evaluate(context) * rhs->Evaluate(context);
            }
        }
        const Node& GetRight() const {
            return *rhs;
        }
        const Node& GetLeft() const {
            return *lhs;
        }
        virtual void Accept(NodeVisitor& visitor) const override {
            visitor.Visit(*this);
         }
    private:
        std::unique_ptr<Node> lhs;
        std::unique_ptr<Node> rhs;
    };

    static std::optional<double> ToNum(const std::string& str) {
        try {
            return std::stod(str);
        } catch(...) {
            return std::nullopt;
        }
    }

    class CellNode : public Node {
    public:
        CellNode(std::string_view cell_name) : cell_pos(Position::FromString(cell_name)) {}
        virtual double Evaluate(const ISheet& context) const override {
            auto* cell_ptr = context.GetCell(cell_pos);
            if(cell_ptr == nullptr) {
                return 0;
            }
            const auto& val = cell_ptr->GetValue();
            if(std::holds_alternative<std::string>(val)) {
                const std::string& str_val = std::get<std::string>(val);
                if(str_val.empty()) {
                    return 0;
                } else if(auto dval = ToNum(str_val); dval.has_value()) {
                    return *dval;
                } else {
                    throw FormulaError(FormulaError::Category::Value);
                }
            } else if(std::holds_alternative<double>(val)) {
                return std::get<double>(val);
            } else {
                throw std::get<FormulaError>(val);
            }  
        }
        Position GetPosition() const noexcept {
            return cell_pos;
        }
        virtual void Accept(NodeVisitor& visitor) const override {
            visitor.Visit(*this);
        }
    private:
        Position cell_pos;
    };

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
    public:
        virtual void Visit(const NumberNode& node) {
            accumulator << node.GetValue();
        }
        virtual void Visit(const UnaryNode<'-'>& node) {
            current_ctx = Context::UNARY_MINUS;
            accumulator << '-';
            node.GetRight().Accept(*this);
        }
        virtual void Visit(const UnaryNode<'+'>& node){
            current_ctx = Context::UNARY_PLUS;
            accumulator << '+';
            node.GetRight().Accept(*this);
        }
        virtual void Visit(const BinaryNode<'+'>& node){
            auto ctx = current_ctx;
            current_ctx = Context::BINARY_PLUS;
            if(ctx == Context::UNARY_MINUS || ctx == Context::UNARY_PLUS || 
            ctx == Context::BINARY_MUL || ctx == Context::BINARY_DIV) {
                accumulator << '(';
            }
            node.GetLeft().Accept(*this);
            accumulator << '+';
            node.GetRight().Accept(*this);
            if(ctx == Context::UNARY_MINUS || ctx == Context::UNARY_PLUS || 
            ctx == Context::BINARY_MUL || ctx == Context::BINARY_DIV) {
                accumulator << ')';
            }
        }
        virtual void Visit(const BinaryNode<'-'>& node){
            auto ctx = current_ctx;
            current_ctx = Context::BINARY_MINUS;
            if(ctx == Context::UNARY_MINUS || ctx == Context::UNARY_PLUS || 
            ctx == Context::BINARY_MUL || ctx == Context::BINARY_DIV) {
                accumulator << '(';
            }
            node.GetLeft().Accept(*this);
            accumulator << '-';
            node.GetRight().Accept(*this);
            if(ctx == Context::UNARY_MINUS || ctx == Context::UNARY_PLUS || 
            ctx == Context::BINARY_MUL || ctx == Context::BINARY_DIV) {
                accumulator << ')';
            }
        }
        virtual void Visit(const BinaryNode<'*'>& node){
            current_ctx = Context::BINARY_MUL;
            node.GetLeft().Accept(*this);
            accumulator << '*';
            node.GetRight().Accept(*this);
        }
        virtual void Visit(const BinaryNode<'/'>& node){
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
        virtual void Visit(const CellNode& node){
            accumulator << node.GetPosition().ToString();
        }
        std::string Get() const {
            return accumulator.str();
        }
    private:
        Context current_ctx = Context::MAIN;
        bool is_divisor = false;
        std::stringstream accumulator;
    };

    std::unique_ptr<Node> ParseFormula(const std::string& in);
}