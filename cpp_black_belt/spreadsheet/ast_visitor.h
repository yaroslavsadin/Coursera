#pragma once

namespace Ast {
    class NumberNode;
    template<char op>
    class UnaryNode;
    template<char op>
    class BinaryNode;
    class CellNode;

    class NodeVisitor {
    public:
        virtual ~NodeVisitor() = default;
        virtual void Visit(const NumberNode& node);
        virtual void Visit(const UnaryNode<'-'>& node);
        virtual void Visit(const UnaryNode<'+'>& node);
        virtual void Visit(const BinaryNode<'+'>& node);
        virtual void Visit(const BinaryNode<'-'>& node);
        virtual void Visit(const BinaryNode<'*'>& node);
        virtual void Visit(const BinaryNode<'/'>& node);
        virtual void Visit(const CellNode& node);
    };

    class NodeVisitorModifier {
    public:
        virtual ~NodeVisitorModifier() = default;
        virtual void Visit(NumberNode& node);
        virtual void Visit(UnaryNode<'-'>& node);
        virtual void Visit(UnaryNode<'+'>& node);
        virtual void Visit(BinaryNode<'+'>& node);
        virtual void Visit(BinaryNode<'-'>& node);
        virtual void Visit(BinaryNode<'*'>& node);
        virtual void Visit(BinaryNode<'/'>& node);
        virtual void Visit(CellNode& node);
    };
}