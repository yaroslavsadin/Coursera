
// Generated from SumMul.g4 by ANTLR 4.7.2


#include "SumMulLexer.h"


using namespace antlr4;


SumMulLexer::SumMulLexer(CharStream *input) : Lexer(input) {
  _interpreter = new atn::LexerATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

SumMulLexer::~SumMulLexer() {
  delete _interpreter;
}

std::string SumMulLexer::getGrammarFileName() const {
  return "SumMul.g4";
}

const std::vector<std::string>& SumMulLexer::getRuleNames() const {
  return _ruleNames;
}

const std::vector<std::string>& SumMulLexer::getChannelNames() const {
  return _channelNames;
}

const std::vector<std::string>& SumMulLexer::getModeNames() const {
  return _modeNames;
}

const std::vector<std::string>& SumMulLexer::getTokenNames() const {
  return _tokenNames;
}

dfa::Vocabulary& SumMulLexer::getVocabulary() const {
  return _vocabulary;
}

const std::vector<uint16_t> SumMulLexer::getSerializedATN() const {
  return _serializedATN;
}

const atn::ATN& SumMulLexer::getATN() const {
  return _atn;
}




// Static vars and initialization.
std::vector<dfa::DFA> SumMulLexer::_decisionToDFA;
atn::PredictionContextCache SumMulLexer::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN SumMulLexer::_atn;
std::vector<uint16_t> SumMulLexer::_serializedATN;

std::vector<std::string> SumMulLexer::_ruleNames = {
  u8"ADD", u8"MUL", u8"NUMBER", u8"WS"
};

std::vector<std::string> SumMulLexer::_channelNames = {
  "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
};

std::vector<std::string> SumMulLexer::_modeNames = {
  u8"DEFAULT_MODE"
};

std::vector<std::string> SumMulLexer::_literalNames = {
  "", u8"'+'", u8"'*'"
};

std::vector<std::string> SumMulLexer::_symbolicNames = {
  "", u8"ADD", u8"MUL", u8"NUMBER", u8"WS"
};

dfa::Vocabulary SumMulLexer::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> SumMulLexer::_tokenNames;

SumMulLexer::Initializer::Initializer() {
  // This code could be in a static initializer lambda, but VS doesn't allow access to private class members from there.
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
    0x2, 0x6, 0x1b, 0x8, 0x1, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 
    0x4, 0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x4, 0x6, 0x4, 0x11, 0xa, 0x4, 0xd, 0x4, 0xe, 0x4, 0x12, 0x3, 
    0x5, 0x6, 0x5, 0x16, 0xa, 0x5, 0xd, 0x5, 0xe, 0x5, 0x17, 0x3, 0x5, 0x3, 
    0x5, 0x2, 0x2, 0x6, 0x3, 0x3, 0x5, 0x4, 0x7, 0x5, 0x9, 0x6, 0x3, 0x2, 
    0x4, 0x3, 0x2, 0x32, 0x3b, 0x5, 0x2, 0xb, 0xc, 0xf, 0xf, 0x22, 0x22, 
    0x2, 0x1c, 0x2, 0x3, 0x3, 0x2, 0x2, 0x2, 0x2, 0x5, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x7, 0x3, 0x2, 0x2, 0x2, 0x2, 0x9, 0x3, 0x2, 0x2, 0x2, 0x3, 0xb, 
    0x3, 0x2, 0x2, 0x2, 0x5, 0xd, 0x3, 0x2, 0x2, 0x2, 0x7, 0x10, 0x3, 0x2, 
    0x2, 0x2, 0x9, 0x15, 0x3, 0x2, 0x2, 0x2, 0xb, 0xc, 0x7, 0x2d, 0x2, 0x2, 
    0xc, 0x4, 0x3, 0x2, 0x2, 0x2, 0xd, 0xe, 0x7, 0x2c, 0x2, 0x2, 0xe, 0x6, 
    0x3, 0x2, 0x2, 0x2, 0xf, 0x11, 0x9, 0x2, 0x2, 0x2, 0x10, 0xf, 0x3, 0x2, 
    0x2, 0x2, 0x11, 0x12, 0x3, 0x2, 0x2, 0x2, 0x12, 0x10, 0x3, 0x2, 0x2, 
    0x2, 0x12, 0x13, 0x3, 0x2, 0x2, 0x2, 0x13, 0x8, 0x3, 0x2, 0x2, 0x2, 
    0x14, 0x16, 0x9, 0x3, 0x2, 0x2, 0x15, 0x14, 0x3, 0x2, 0x2, 0x2, 0x16, 
    0x17, 0x3, 0x2, 0x2, 0x2, 0x17, 0x15, 0x3, 0x2, 0x2, 0x2, 0x17, 0x18, 
    0x3, 0x2, 0x2, 0x2, 0x18, 0x19, 0x3, 0x2, 0x2, 0x2, 0x19, 0x1a, 0x8, 
    0x5, 0x2, 0x2, 0x1a, 0xa, 0x3, 0x2, 0x2, 0x2, 0x5, 0x2, 0x12, 0x17, 
    0x3, 0x8, 0x2, 0x2, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

SumMulLexer::Initializer SumMulLexer::_init;
