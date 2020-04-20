
// Generated from SumMul.g4 by ANTLR 4.8


#include "SumMulListener.h"

#include "SumMulParser.h"


using namespace antlrcpp;
using namespace antlr4;

SumMulParser::SumMulParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

SumMulParser::~SumMulParser() {
  delete _interpreter;
}

std::string SumMulParser::getGrammarFileName() const {
  return "SumMul.g4";
}

const std::vector<std::string>& SumMulParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& SumMulParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- MainContext ------------------------------------------------------------------

SumMulParser::MainContext::MainContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SumMulParser::ExprContext* SumMulParser::MainContext::expr() {
  return getRuleContext<SumMulParser::ExprContext>(0);
}

tree::TerminalNode* SumMulParser::MainContext::EOF() {
  return getToken(SumMulParser::EOF, 0);
}


size_t SumMulParser::MainContext::getRuleIndex() const {
  return SumMulParser::RuleMain;
}

void SumMulParser::MainContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SumMulListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMain(this);
}

void SumMulParser::MainContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SumMulListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMain(this);
}

SumMulParser::MainContext* SumMulParser::main() {
  MainContext *_localctx = _tracker.createInstance<MainContext>(_ctx, getState());
  enterRule(_localctx, 0, SumMulParser::RuleMain);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(4);
    expr(0);
    setState(5);
    match(SumMulParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

SumMulParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SumMulParser::ExprContext::NUMBER() {
  return getToken(SumMulParser::NUMBER, 0);
}

std::vector<SumMulParser::ExprContext *> SumMulParser::ExprContext::expr() {
  return getRuleContexts<SumMulParser::ExprContext>();
}

SumMulParser::ExprContext* SumMulParser::ExprContext::expr(size_t i) {
  return getRuleContext<SumMulParser::ExprContext>(i);
}

tree::TerminalNode* SumMulParser::ExprContext::MUL() {
  return getToken(SumMulParser::MUL, 0);
}

tree::TerminalNode* SumMulParser::ExprContext::ADD() {
  return getToken(SumMulParser::ADD, 0);
}


size_t SumMulParser::ExprContext::getRuleIndex() const {
  return SumMulParser::RuleExpr;
}

void SumMulParser::ExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SumMulListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpr(this);
}

void SumMulParser::ExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SumMulListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpr(this);
}


SumMulParser::ExprContext* SumMulParser::expr() {
   return expr(0);
}

SumMulParser::ExprContext* SumMulParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SumMulParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  SumMulParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 2;
  enterRecursionRule(_localctx, 2, SumMulParser::RuleExpr, precedence);

    

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(8);
    match(SumMulParser::NUMBER);
    _ctx->stop = _input->LT(-1);
    setState(18);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(16);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpr);
          setState(10);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(11);
          match(SumMulParser::MUL);
          setState(12);
          expr(4);
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpr);
          setState(13);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(14);
          match(SumMulParser::ADD);
          setState(15);
          expr(3);
          break;
        }

        } 
      }
      setState(20);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

bool SumMulParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 1: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool SumMulParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 3);
    case 1: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> SumMulParser::_decisionToDFA;
atn::PredictionContextCache SumMulParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN SumMulParser::_atn;
std::vector<uint16_t> SumMulParser::_serializedATN;

std::vector<std::string> SumMulParser::_ruleNames = {
  "main", "expr"
};

std::vector<std::string> SumMulParser::_literalNames = {
  "", "'+'", "'*'"
};

std::vector<std::string> SumMulParser::_symbolicNames = {
  "", "ADD", "MUL", "NUMBER", "WS"
};

dfa::Vocabulary SumMulParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> SumMulParser::_tokenNames;

SumMulParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x6, 0x18, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7, 0x3, 0x13, 0xa, 0x3, 0xc, 0x3, 
    0xe, 0x3, 0x16, 0xb, 0x3, 0x3, 0x3, 0x2, 0x3, 0x4, 0x4, 0x2, 0x4, 0x2, 
    0x2, 0x2, 0x17, 0x2, 0x6, 0x3, 0x2, 0x2, 0x2, 0x4, 0x9, 0x3, 0x2, 0x2, 
    0x2, 0x6, 0x7, 0x5, 0x4, 0x3, 0x2, 0x7, 0x8, 0x7, 0x2, 0x2, 0x3, 0x8, 
    0x3, 0x3, 0x2, 0x2, 0x2, 0x9, 0xa, 0x8, 0x3, 0x1, 0x2, 0xa, 0xb, 0x7, 
    0x5, 0x2, 0x2, 0xb, 0x14, 0x3, 0x2, 0x2, 0x2, 0xc, 0xd, 0xc, 0x5, 0x2, 
    0x2, 0xd, 0xe, 0x7, 0x4, 0x2, 0x2, 0xe, 0x13, 0x5, 0x4, 0x3, 0x6, 0xf, 
    0x10, 0xc, 0x4, 0x2, 0x2, 0x10, 0x11, 0x7, 0x3, 0x2, 0x2, 0x11, 0x13, 
    0x5, 0x4, 0x3, 0x5, 0x12, 0xc, 0x3, 0x2, 0x2, 0x2, 0x12, 0xf, 0x3, 0x2, 
    0x2, 0x2, 0x13, 0x16, 0x3, 0x2, 0x2, 0x2, 0x14, 0x12, 0x3, 0x2, 0x2, 
    0x2, 0x14, 0x15, 0x3, 0x2, 0x2, 0x2, 0x15, 0x5, 0x3, 0x2, 0x2, 0x2, 
    0x16, 0x14, 0x3, 0x2, 0x2, 0x2, 0x4, 0x12, 0x14, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

SumMulParser::Initializer SumMulParser::_init;
