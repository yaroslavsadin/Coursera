#pragma once
#include <iostream>
#include "common.h"
#include <optional>
#include <sstream>
#include "ast_visitor.h"

namespace Ast {
    class NodeVisitor;

    class Node {
    public:
        virtual double Evaluate(const ISheet& context) const = 0;
        virtual void Accept(NodeVisitor& visitor) const = 0;
        virtual void Accept(NodeVisitorModifier& visitor) = 0;
    };

    std::unique_ptr<Node> ParseFormula(const std::string& in);

    class NumberNode : public Node {
    public:
        NumberNode(double value) : value(value) {}
        virtual double Evaluate(const ISheet& context) const noexcept override  {
            return value;
        }
        virtual void Accept(NodeVisitor& visitor) const override {
            visitor.Visit(*this);
        }
        virtual void Accept(NodeVisitorModifier& visitor) override {
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
        Node& GetRight() {
            return *rhs;
        }
        virtual void Accept(NodeVisitor& visitor) const override {
            visitor.Visit(*this);
        }
        virtual void Accept(NodeVisitorModifier& visitor) override {
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
        Node& GetRight() {
            return *rhs;
        }
         Node& GetLeft() {
            return *lhs;
        }
        virtual void Accept(NodeVisitor& visitor) const override {
            visitor.Visit(*this);
        }
        virtual void Accept(NodeVisitorModifier& visitor) override {
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
        void SetPosition(Position pos) noexcept {
            cell_pos = pos;
        }
        virtual void Accept(NodeVisitor& visitor) const override {
            visitor.Visit(*this);
        }
        virtual void Accept(NodeVisitorModifier& visitor) override {
            visitor.Visit(*this);
        }
    private:
        Position cell_pos;
    };
}