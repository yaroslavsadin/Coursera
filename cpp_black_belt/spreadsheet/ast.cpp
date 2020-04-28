#include "ast.h"
#include "FormulaLexer.h"
#include "FormulaParser.h"
#include "FormulaBaseListener.h"

namespace Ast {
    void ParseFormula(std::string in) {
        antlr4::ANTLRInputStream input(in);
        FormulaLexer lexer(&input);
        antlr4::CommonTokenStream tokens(&lexer);
        FormulaParser parser(&tokens);
        antlr4::tree::ParseTree* tree = parser.main();
        FormulaBaseListener listener;
        antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
    }
}