#include "FormulaLexer.h"
#include "FormulaParser.h"
#include "FormulaBaseListener.h"
#include "ast_node.h"

class NodeTreeListener : public FormulaBaseListener {
private:
  std::stack<std::unique_ptr<Ast::Node>> builder;
public:

  std::unique_ptr<Ast::Node> getAst() {
    auto res = std::move(builder.top());
    builder.pop();
    assert(builder.empty());
    return res;
  }

  virtual void enterMain(FormulaParser::MainContext * ctx) override;
  virtual void exitMain(FormulaParser::MainContext * ctx) override;
  virtual void enterUnaryOp(FormulaParser::UnaryOpContext * ctx) override;
  virtual void exitUnaryOp(FormulaParser::UnaryOpContext * ctx) override;
  virtual void enterParens(FormulaParser::ParensContext * ctx) override;
  virtual void exitParens(FormulaParser::ParensContext * ctx) override;
  virtual void enterCell(FormulaParser::CellContext * ctx) override;
  virtual void exitCell(FormulaParser::CellContext * ctx) override;
  virtual void enterLiteral(FormulaParser::LiteralContext * ctx) override;
  virtual void exitLiteral(FormulaParser::LiteralContext * ctx) override;
  virtual void enterBinaryOp(FormulaParser::BinaryOpContext * ctx) override;
  virtual void exitBinaryOp(FormulaParser::BinaryOpContext * ctx) override;
  virtual void enterEveryRule(antlr4::ParserRuleContext * ctx) override;
  virtual void exitEveryRule(antlr4::ParserRuleContext * ctx) override;
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override;
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override;

};

void NodeTreeListener::enterMain(FormulaParser::MainContext * ctx)  { }
void NodeTreeListener::exitMain(FormulaParser::MainContext * ctx)  { }

void NodeTreeListener::enterUnaryOp(FormulaParser::UnaryOpContext * ctx)  { }
void NodeTreeListener::exitUnaryOp(FormulaParser::UnaryOpContext * ctx)  {
    auto rhs = std::move(builder.top());
    builder.pop();
    if(ctx->getText()[0] == '-') {
        builder.push(
            std::make_unique<Ast::UnaryNode<'-'>>(
                std::move(rhs)
            )
        );
    } else {
        builder.push(
            std::make_unique<Ast::UnaryNode<'+'>>(
                std::move(rhs)
            )
        );
    }
}

void NodeTreeListener::enterParens(FormulaParser::ParensContext * ctx)  { }
void NodeTreeListener::exitParens(FormulaParser::ParensContext * ctx)  { }

void NodeTreeListener::enterCell(FormulaParser::CellContext * ctx)  { }
void NodeTreeListener::exitCell(FormulaParser::CellContext * ctx)  {   
    if(ctx->exception) {
        std::cerr << "Exception" << std::endl;
    }
    builder.push(std::make_unique<Ast::CellNode>(ctx->getText()));
}

void NodeTreeListener::enterLiteral(FormulaParser::LiteralContext * ctx)  { }
void NodeTreeListener::exitLiteral(FormulaParser::LiteralContext * ctx)  {
    builder.push(
        std::make_unique<Ast::NumberNode>(
            std::atof(ctx->getText().c_str())
        )
    ); 
}

void NodeTreeListener::enterBinaryOp(FormulaParser::BinaryOpContext * ctx)  { }
void NodeTreeListener::exitBinaryOp(FormulaParser::BinaryOpContext * ctx)  {
    size_t ttype = FormulaParser::ADD;
    for(; ttype <= FormulaParser::DIV; ttype++) {
        if(auto token = ctx->getToken(ttype,0); token) 
            break;
    }
    auto rhs = std::move(builder.top()); builder.pop();
    auto lhs = std::move(builder.top()); builder.pop();
    switch (ttype)
    {
    case FormulaParser::ADD:
        builder.push(std::make_unique<Ast::BinaryNode<'+'>>(std::move(lhs),std::move(rhs)));
        break;
    case FormulaParser::SUB:
        builder.push(std::make_unique<Ast::BinaryNode<'-'>>(std::move(lhs),std::move(rhs)));
        break;
    case FormulaParser::MUL:
        builder.push(std::make_unique<Ast::BinaryNode<'*'>>(std::move(lhs),std::move(rhs)));
        break;
    case FormulaParser::DIV:
        builder.push(std::make_unique<Ast::BinaryNode<'/'>>(std::move(lhs),std::move(rhs)));
        break;
    default:
        throw std::out_of_range("Wrong binary op");
        break;
    }
}

void NodeTreeListener::enterEveryRule(antlr4::ParserRuleContext * ctx)  { }
void NodeTreeListener::exitEveryRule(antlr4::ParserRuleContext * ctx)  { }
void NodeTreeListener::visitTerminal(antlr4::tree::TerminalNode * /*node*/)  { }
void NodeTreeListener::visitErrorNode(antlr4::tree::ErrorNode * ctx)  {
}

class BailErrorListener : public antlr4::BaseErrorListener {
public:
    void syntaxError(antlr4::Recognizer* /* recognizer */,
                     antlr4::Token* /* offendingSymbol */, size_t /* line */,
                     size_t /* charPositionInLine */, const std::string& msg,
                     std::exception_ptr /* e */
    ) override {
        throw std::runtime_error("Error when lexing: " + msg);
    }
};

namespace Ast {
    std::unique_ptr<Node> ParseFormula(const std::string& in) {
        antlr4::ANTLRInputStream input(in);

        FormulaLexer lexer(&input);
        BailErrorListener error_listener;
        lexer.removeErrorListeners();
        lexer.addErrorListener(&error_listener);
        
        antlr4::CommonTokenStream tokens(&lexer);

        FormulaParser parser(&tokens);
        auto error_handler = std::make_shared<antlr4::BailErrorStrategy>();
        parser.setErrorHandler(error_handler);
        parser.removeErrorListeners();

        try {
            antlr4::tree::ParseTree* tree = parser.main();
            NodeTreeListener listener;
            antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
            return listener.getAst();
        } catch(std::exception& err) {
            throw FormulaException(err.what());
        }
    }
}