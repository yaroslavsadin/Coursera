
// Generated from SumMul.g4 by ANTLR 4.7.2

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

  virtual void enterMul(SumMulParser::MulContext *ctx) = 0;
  virtual void exitMul(SumMulParser::MulContext *ctx) = 0;

  virtual void enterNumber(SumMulParser::NumberContext *ctx) = 0;
  virtual void exitNumber(SumMulParser::NumberContext *ctx) = 0;

  virtual void enterAdd(SumMulParser::AddContext *ctx) = 0;
  virtual void exitAdd(SumMulParser::AddContext *ctx) = 0;


};

