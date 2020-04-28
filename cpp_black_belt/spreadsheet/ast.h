#pragma once
#include <iostream>
#include "common.h"
#include <optional>

namespace Ast {
    class Node {
    public:
        virtual double Evaluate(const ISheet& context) const = 0;
    };

    class NumberNode : public Node {
    public:
        NumberNode(double value) : value(value) {}
        virtual double Evaluate(const ISheet& context) const noexcept override  {
            return value;
        }
    private:
        double value;
    };

    template<char op>
    class BinaryNode : public Node {
    public:
        BinaryNode(const Node& lhs, const Node& rhs) : lhs(lhs), rhs(rhs) {}
        virtual double Evaluate(const ISheet& context) const override {
            static_assert(op == '+' || op == '-' || op == '/' || op == '*');
            if constexpr(op == '+') {
                return lhs.Evaluate(context) + rhs.Evaluate(context);
            } else if constexpr(op == '-') {
                return lhs.Evaluate(context) - rhs.Evaluate(context);
            } else if constexpr(op == '/') {
                auto rhs_val = rhs.Evaluate(context);
                if(!rhs_val) {
                    throw FormulaError(FormulaError::Category::Div0);
                }
                return lhs.Evaluate(context) / rhs_val;
            } else if constexpr(op == '*') {
                return lhs.Evaluate(context) * rhs.Evaluate(context);
            }
        }
    private:
        const Node& lhs;
        const Node& rhs;
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
        CellNode(Position cell_pos) : cell_pos(cell_pos) {}
        virtual double Evaluate(const ISheet& context) const override {
            auto* cell_ptr = context.GetCell(cell_pos);
            if(cell_ptr == nullptr) {
                throw FormulaError(FormulaError::Category::Ref);
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
    private:
        Position cell_pos;
    };

    void ParseFormula(std::string in);
}