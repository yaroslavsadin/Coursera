#include "json.h"
#include "misc.h"
#include <cctype>
#include <cmath>

using namespace std;

namespace Json {

  Document::Document(Node root) : root(move(root)) {
  }

  const Node& Document::GetRoot() const {
    return root;
  }

  Node LoadNode(istream& input);

  Node LoadArray(istream& input) {
    vector<Node> result;

    for (char c; input >> c && c != ']'; ) {
      if (c != ',') {
        input.putback(c);
      }
      result.push_back(LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadNum(istream& input) {
    int result = 0;
    double fract = 0;
    while (isdigit(input.peek())) {
      result *= 10;
      result += input.get() - '0';
    }
    if(input.peek() == '.') {
      input.ignore(1);
      int count = 0;
      while (isdigit(input.peek())) {
        fract *= 10;
        fract += input.get() - '0';
        count++;
      }
      fract /= pow(10,count);
      return Node(result + fract);
    } else {
      return Node(result);
    }
  }

  Node LoadBool(istream& input) {
    string str;
    while(isalpha(input.peek())) {
      str.push_back(input.get());
    }
    if(str == "false") {
      return Node(false);
    } else {
      return Node(true);
    }
  }

  Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
  }

  Node LoadDict(istream& input) {
    map<string, Node> result;

    for (char c; input >> c && c != '}'; ) {
      if (c == ',') {
        input >> c;
      }

      string key = LoadString(input).AsString();
      input >> c;
      result.emplace(move(key), LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
      return LoadArray(input);
    } else if (c == '{') {
      return LoadDict(input);
    } else if (c == '"') {
      return LoadString(input);
    } else if (isalpha(c)) {
      input.putback(c);
      return LoadBool(input);
    } else {
      input.putback(c);
      return LoadNum(input);
    }
  }

  Document Load(istream& input) {
    return Document{LoadNode(input)};
  }

}
