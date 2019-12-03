#include "lexer.h"

#include <algorithm>
// #include <charconv>
#include <unordered_map>

using namespace std;

namespace Parse {

bool operator == (const Token& lhs, const Token& rhs) {
  using namespace TokenType;

  if (lhs.index() != rhs.index()) {
    return false;
  }
  if (lhs.Is<Char>()) {
    return lhs.As<Char>().value == rhs.As<Char>().value;
  } else if (lhs.Is<Number>()) {
    return lhs.As<Number>().value == rhs.As<Number>().value;
  } else if (lhs.Is<String>()) {
    return lhs.As<String>().value == rhs.As<String>().value;
  } else if (lhs.Is<Id>()) {
    return lhs.As<Id>().value == rhs.As<Id>().value;
  } else {
    return true;
  }
}

std::ostream& operator << (std::ostream& os, const Token& rhs) {
  using namespace TokenType;

#define VALUED_OUTPUT(type) \
  if (auto p = rhs.TryAs<type>()) return os << #type << '{' << p->value << '}';

  VALUED_OUTPUT(Number);
  VALUED_OUTPUT(Id);
  VALUED_OUTPUT(String);
  VALUED_OUTPUT(Char);

#undef VALUED_OUTPUT

#define UNVALUED_OUTPUT(type) \
    if (rhs.Is<type>()) return os << #type;

  UNVALUED_OUTPUT(Class);
  UNVALUED_OUTPUT(Return);
  UNVALUED_OUTPUT(If);
  UNVALUED_OUTPUT(Else);
  UNVALUED_OUTPUT(Def);
  UNVALUED_OUTPUT(Newline);
  UNVALUED_OUTPUT(Print);
  UNVALUED_OUTPUT(Indent);
  UNVALUED_OUTPUT(Dedent);
  UNVALUED_OUTPUT(And);
  UNVALUED_OUTPUT(Or);
  UNVALUED_OUTPUT(Not);
  UNVALUED_OUTPUT(Eq);
  UNVALUED_OUTPUT(NotEq);
  UNVALUED_OUTPUT(LessOrEq);
  UNVALUED_OUTPUT(GreaterOrEq);
  UNVALUED_OUTPUT(None);
  UNVALUED_OUTPUT(True);
  UNVALUED_OUTPUT(False);
  UNVALUED_OUTPUT(Eof);

#undef UNVALUED_OUTPUT

  return os << "Unknown token :(";
}

static std::unordered_map<std::string_view,Token> keywords {
  { "class", Token(TokenType::Class{}) },
  { "return", Token(TokenType::Return{}) },
  { "if", Token(TokenType::If{}) },
  { "else", Token(TokenType::Else{}) },
  { "def", Token(TokenType::Def{}) },
  { "print",Token(TokenType::Print{}) },
  { "and", Token(TokenType::And{}) },
  { "or", Token(TokenType::Or{}) },
  { "not", Token(TokenType::Not{}) },
  { "None", Token(TokenType::None{}) },
  { "True", Token(TokenType::True{}) },
  { "False", Token(TokenType::False{}) }
};

static std::unordered_map<std::string_view,Token> compares {
  { "==", Token(TokenType::Eq{}) },
  { "!=", Token(TokenType::NotEq{}) },
  { "<=", Token(TokenType::LessOrEq{}) },
  { ">=", Token(TokenType::GreaterOrEq{}) }
};

Lexer::Lexer(std::istream& input) : 
input(input), tokens(), current_token(0)
{
  std::string str;
  size_t current_indent = 0;
  // Main loop
  while(getline(input,str)) {
    // Getting string into a view
    string_view view = str;
    // Checking for tabs to identify indent
    size_t indent = 0;
    while(isblank(view[0])) {
      view.remove_prefix(1);
      indent++;
    }
    // If have only the whitespaces - ignore the whole line
    if(view.empty()) {
      continue;
    }
    // Adjusting current indent
    if(current_indent < indent) {
      while(current_indent != indent) {  
        tokens.push_back(Token(TokenType::Indent{}));
        current_indent +=2;
      }
    } else if(current_indent > indent) {
      while(current_indent != indent) {      
        tokens.push_back(Token(TokenType::Dedent{}));
        current_indent -=2;
      }
    }

    // Checking on actual tokens
    while(view.size()) {
      // Removing any whitespaces between tokens
      while(isblank(view[0])) {
        view.remove_prefix(1);
      }
      if(!view.size()) {
        break;
      }
      
      // Keyword or identifier
      if(isalpha(view[0]) || view[0] == '_') {
        size_t pos = 0;
        while(isalnum(view[pos]) || view[pos] == '_') { pos++; }
        // Check if this is a keyword or an id...
        if(keywords.count(view.substr(0,pos))) {
          tokens.push_back(keywords.at(view.substr(0,pos)));
        } else {
          tokens.push_back(Token(TokenType::Id{std::string(view.substr(0,pos))}));
        }
        view.remove_prefix(pos);

      // A number
      } else if(isalnum(view[0])) {
        size_t pos = 0;
        while(isalnum(view[pos])) { pos++; }
        tokens.push_back(Token(TokenType::Number{stoi(std::string(view.substr(0,pos)))}));
        view.remove_prefix(pos);
      // A string  
      } else if(view[0] == '\'' || view[0] == '\"') {
        char delim = view[0];
        size_t pos = view.find(delim,1);
        tokens.push_back(Token(TokenType::String{std::string(view.substr(1,pos-1))}));
        view.remove_prefix(pos+1);
      // Either comparator or Char 
      } else {
        if(view.size() >= 2 && compares.count(view.substr(0,2))) {
          tokens.push_back(compares.at(view.substr(0,2)));
          view.remove_prefix(2);
        } else {
          tokens.push_back(Token(TokenType::Char{view[0]}));
          view.remove_prefix(1);
        }
      }
    }
    // Emit newline
    tokens.push_back(Token(TokenType::Newline{}));
  }
  while(current_indent) {
    tokens.push_back(Token(TokenType::Dedent{}));
    current_indent -=2;
  }
  tokens.push_back(Token(TokenType::Eof{}));
}

const Token& Lexer::CurrentToken() const {
  return tokens[current_token];
}

Token Lexer::NextToken() {
  return tokens[
    (current_token < tokens.size() - 1) ? ++current_token : tokens.size() - 1
  ];
}

} /* namespace Parse */
