#include "FormulaLexer.h"
#include "FormulaParser.h"
#include "FormulaBaseListener.h"

void FormulaBaseListener::enterMain(FormulaParser::MainContext * ctx)  { }
void FormulaBaseListener::exitMain(FormulaParser::MainContext * ctx)  { }

void FormulaBaseListener::enterUnaryOp(FormulaParser::UnaryOpContext * ctx)  { }
void FormulaBaseListener::exitUnaryOp(FormulaParser::UnaryOpContext * ctx)  {
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

void FormulaBaseListener::enterParens(FormulaParser::ParensContext * ctx)  { }
void FormulaBaseListener::exitParens(FormulaParser::ParensContext * ctx)  { }

void FormulaBaseListener::enterCell(FormulaParser::CellContext * ctx)  { }
void FormulaBaseListener::exitCell(FormulaParser::CellContext * ctx)  {   
    if(ctx->exception) {
        std::cerr << "Exception" << std::endl;
    }
    builder.push(std::make_unique<Ast::CellNode>(ctx->getText()));
}

void FormulaBaseListener::enterLiteral(FormulaParser::LiteralContext * ctx)  { }
void FormulaBaseListener::exitLiteral(FormulaParser::LiteralContext * ctx)  {
    builder.push(
        std::make_unique<Ast::NumberNode>(
            std::atof(ctx->getText().c_str())
        )
    ); 
}

void FormulaBaseListener::enterBinaryOp(FormulaParser::BinaryOpContext * ctx)  { }
void FormulaBaseListener::exitBinaryOp(FormulaParser::BinaryOpContext * ctx)  {
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

void FormulaBaseListener::enterEveryRule(antlr4::ParserRuleContext * ctx)  { }
void FormulaBaseListener::exitEveryRule(antlr4::ParserRuleContext * ctx)  { }
void FormulaBaseListener::visitTerminal(antlr4::tree::TerminalNode * /*node*/)  { }
void FormulaBaseListener::visitErrorNode(antlr4::tree::ErrorNode * ctx)  {
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
            FormulaBaseListener listener;
            antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
            return listener.getAst();
        } catch(std::exception& err) {
            throw FormulaException(err.what());
        }
    }
}