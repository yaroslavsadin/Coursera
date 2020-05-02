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
}