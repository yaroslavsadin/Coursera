#include "ast_visitor.h"
#include "ast_node.h"

namespace Ast {
    void NodeVisitor::Visit(const NumberNode& node) {

    }
    void NodeVisitor::Visit(const UnaryNode<'-'>& node)  {
            node.GetRight().Accept(*this);
    }
    void NodeVisitor::Visit(const UnaryNode<'+'>& node)  {
        node.GetRight().Accept(*this);
    }
    void NodeVisitor::Visit(const BinaryNode<'+'>& node)  {
        node.GetLeft().Accept(*this);
        node.GetRight().Accept(*this);
    }
    void NodeVisitor::Visit(const BinaryNode<'-'>& node)  {
        node.GetLeft().Accept(*this);
        node.GetRight().Accept(*this);
    }
    void NodeVisitor::Visit(const BinaryNode<'*'>& node)  {
        node.GetLeft().Accept(*this);
        node.GetRight().Accept(*this);
    }
    void NodeVisitor::Visit(const BinaryNode<'/'>& node)  {
        node.GetLeft().Accept(*this);
        node.GetRight().Accept(*this);
    }
    void NodeVisitor::Visit(const CellNode& node) {

    }
    void NodeVisitorModifier::Visit(NumberNode& node) {

    }
    void NodeVisitorModifier::Visit(UnaryNode<'-'>& node)  {
            node.GetRight().Accept(*this);
    }
    void NodeVisitorModifier::Visit(UnaryNode<'+'>& node)  {
        node.GetRight().Accept(*this);
    }
    void NodeVisitorModifier::Visit(BinaryNode<'+'>& node)  {
        node.GetLeft().Accept(*this);
        node.GetRight().Accept(*this);
    }
    void NodeVisitorModifier::Visit(BinaryNode<'-'>& node)  {
        node.GetLeft().Accept(*this);
        node.GetRight().Accept(*this);
    }
    void NodeVisitorModifier::Visit(BinaryNode<'*'>& node)  {
        node.GetLeft().Accept(*this);
        node.GetRight().Accept(*this);
    }
    void NodeVisitorModifier::Visit(BinaryNode<'/'>& node)  {
        node.GetLeft().Accept(*this);
        node.GetRight().Accept(*this);
    }
    void NodeVisitorModifier::Visit(CellNode& node) {

    }
}