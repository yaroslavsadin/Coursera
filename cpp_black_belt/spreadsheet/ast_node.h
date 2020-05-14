#pragma once
#include <iostream>
#include "common.h"
#include <optional>
#include <sstream>
#include <cmath>
#include "ast_visitor.h"

namespace Ast {
    class NodeVisitor;

    class Node {
    public:
        virtual double Evaluate(const ISheet& context) const = 0;
        virtual void Accept(NodeVisitor& visitor) const = 0;
        virtual void Accept(NodeVisitorModifier& visitor) = 0;
        virtual ~Node() = default;
    };

    std::unique_ptr<Node> ParseFormula(const std::string& in);

    class NumberNode : public Node {
    public:
        NumberNode(double value) : value(value) {}
        virtual double Evaluate(const ISheet& context) const override  {
            if(!std::isfinite(value)) {
                throw FormulaError(FormulaError::Category::Div0);
            }
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
                auto res = rhs->Evaluate(context);
                if(!std::isfinite(res)) {
                    throw FormulaError(FormulaError::Category::Div0);
                }
                return res;
            } else if constexpr(op == '-') {
                auto res = - rhs->Evaluate(context);
                if(!std::isfinite(res)) {
                    throw FormulaError(FormulaError::Category::Div0);
                }
                return res;
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
                auto res = lhs->Evaluate(context) + rhs->Evaluate(context);
                if(!std::isfinite(res)) {
                    throw FormulaError(FormulaError::Category::Div0);
                }
                return res;
            } else if constexpr(op == '-') {
                auto res = lhs->Evaluate(context) - rhs->Evaluate(context);
                if(!std::isfinite(res)) {
                    throw FormulaError(FormulaError::Category::Div0);
                }
                return res;
            } else if constexpr(op == '/') {
                auto res = lhs->Evaluate(context) / rhs->Evaluate(context);
                if(!std::isfinite(res)) {
                    throw FormulaError(FormulaError::Category::Div0);
                }
                return res;
            } else if constexpr(op == '*') {
                auto res = lhs->Evaluate(context) * rhs->Evaluate(context);
                if(!std::isfinite(res)) {
                    throw FormulaError(FormulaError::Category::Div0);
                }
                return res;
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
        std::istringstream iss(str);
        double f;
        iss >> std::noskipws >> f;
        if(iss.eof() && !iss.fail()) {
            return f;
        } else {
            return std::nullopt;
        }
    }

    class CellNode : public Node {
    public:
        static constexpr Position REF{-1,-1};
        CellNode(std::string_view cell_name) : cell_pos(Position::FromString(cell_name)) {}
        virtual double Evaluate(const ISheet& context) const override {
            // std::cerr << "CellNode::Evaluate" << std::endl;
            if(cell_pos == CellNode::REF) {
                throw FormulaError(FormulaError::Category::Ref);
            }
            const auto* cell_ptr = context.GetCell(cell_pos);
            if(cell_ptr == nullptr) {
                return 0;
            }
            const auto& val = cell_ptr->GetValue();
            if(std::holds_alternative<std::string>(val)) {
                const std::string& str_val = std::get<std::string>(val);
                if(str_val.empty()) {
                    return 0;
                } else if(auto dval = ToNum(str_val); dval.has_value()) {
                    if(!std::isfinite(*dval)) {
                        throw FormulaError(FormulaError::Category::Div0);
                    }
                    return *dval;
                } else {
                    throw FormulaError(FormulaError::Category::Value);
                }
            } else if(std::holds_alternative<double>(val)) {
                if(!std::isfinite(std::get<double>(val))) {
                    throw FormulaError(FormulaError::Category::Div0);
                }
                return std::get<double>(val);
            } else {
                throw std::get<FormulaError>(val);
            }
        }
        Position GetPosition() const noexcept {
            return cell_pos;
        }
        Position& GetPosition() noexcept {
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