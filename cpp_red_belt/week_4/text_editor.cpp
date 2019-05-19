#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include "test_runner.h"
using namespace std;

class Editor {
 public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor();
  void Left();
  void Right();
  void Insert(char token);
  void Cut(size_t tokens = 1);
  void Copy(size_t tokens = 1);
  void Paste();
  string GetText() const;
 private:
 // Could use list and assign()
  vector<char> front_;
  deque<char> back_;
  vector<char> buffer;
};

Editor::Editor() {
}

// O(1)
void Editor::Left() {
  if(front_.size()) {
    back_.push_front(front_.back());
    front_.pop_back();
  }
 }

// O(1)
void Editor::Right() {
  if(back_.size()) {
    front_.push_back(back_.front());
    back_.pop_front();
  }
}

// O(1)
void Editor::Insert(char token) {
  front_.push_back(token);
}

// O(N)
void Editor::Copy(size_t tokens) {
  buffer.resize(tokens);
  copy(back_.begin(),back_.begin()+tokens,buffer.begin());
}

// O(N)
void Editor::Cut(size_t tokens) {
  Copy(tokens);
  if(back_.size() > tokens)  
    back_.erase(back_.begin(),back_.begin()+tokens);
  else
    back_.clear();
}

// O(N)
void Editor::Paste() {
  front_.reserve(front_.size()+buffer.size());
  front_.insert(front_.end(),buffer.begin(),buffer.end());
}

// O(N)
string Editor::GetText() const {
  return string(front_.begin(),front_.end()) + string(back_.begin(),back_.end());
}

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);
    
    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;
    
    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }
  
  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");
  
  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}