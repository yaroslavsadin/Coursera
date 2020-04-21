
// Generated from SumMul.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "SumMulListener.h"


/**
 * This class provides an empty implementation of SumMulListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  SumMulBaseListener : public SumMulListener {
public:

  virtual void enterMain(SumMulParser::MainContext * /*ctx*/) override { }
  virtual void exitMain(SumMulParser::MainContext * /*ctx*/) override { }

  virtual void enterMul(SumMulParser::MulContext * /*ctx*/) override { }
  virtual void exitMul(SumMulParser::MulContext * /*ctx*/) override { }

  virtual void enterNumber(SumMulParser::NumberContext * /*ctx*/) override { }
  virtual void exitNumber(SumMulParser::NumberContext * /*ctx*/) override { }

  virtual void enterAdd(SumMulParser::AddContext * /*ctx*/) override { }
  virtual void exitAdd(SumMulParser::AddContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

