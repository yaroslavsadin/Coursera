#include <string>
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
  using ContainerT = deque<char>;
  // void UpdateTextView() {
  //   text_view = string_view(&text[0],text.size());
  // }
  ContainerT text;
  ContainerT::iterator cursor;
  ContainerT::iterator copy_;
  // string_view text_view;
  vector<char> buffer;
};

Editor::Editor() : buffer(), text(), cursor(text.begin()) {
}

// O(1)
void Editor::Left() {
  if(cursor != text.begin()) {
    cursor--;
  }
 }

// O(1)
void Editor::Right() {
  if(cursor != text.end()) {
    cursor++;
  }
}

// O(1) + O(N/2)
void Editor::Insert(char token) {
  cursor = text.insert(cursor,token);
  cursor++;
  // UpdateTextView();
}

// O(N)
void Editor::Copy(size_t tokens) {
  copy(cursor,cursor+tokens,back_inserter(buffer));
  // buffer = res;
}

// O(N)
void Editor::Cut(size_t tokens) {
  Copy(tokens);
  if(text.end() - cursor < tokens) {  
    cursor = text.erase(cursor,text.end());
  } else {
    cursor = text.erase(cursor,cursor+tokens);
  }
  // UpdateTextView();
}

// O(1) + O(N/2)
void Editor::Paste() {
  cursor = text.insert(cursor,buffer.begin(),buffer.end());
  cursor += buffer.size();
  // UpdateTextView();
}

// O(N)
string Editor::GetText() const {
  return string(text.begin(),text.end());
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