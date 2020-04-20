
// Generated from SumMul.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "SumMulParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by SumMulParser.
 */
class  SumMulListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterMain(SumMulParser::MainContext *ctx) = 0;
  virtual void exitMain(SumMulParser::MainContext *ctx) = 0;

  virtual void enterExpr(SumMulParser::ExprContext *ctx) = 0;
  virtual void exitExpr(SumMulParser::ExprContext *ctx) = 0;


};

