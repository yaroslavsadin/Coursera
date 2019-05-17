#include <string>
#include <list>
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
  using ContainerT = list<char>;
  ContainerT text;
  ContainerT::iterator cursor;
  ContainerT::iterator buffer_start;
  ContainerT::iterator buffer_end;
  ContainerT::const_iterator text_end;
};

Editor::Editor() : cursor(text.begin()), buffer_start(text.end()), buffer_end(text.end()), text_end(text.end()) {
}

// O(1)
void Editor::Left() {
  if(cursor != text.begin()) {
    cursor--;
  }
}

// O(1)
void Editor::Right() {
  if(cursor != text_end) {
    cursor++;
  }
}

// O(1)
void Editor::Insert(char token) {
  cursor = text.insert(cursor,token);
  cursor++;
}

// O(N)
void Editor::Copy(size_t tokens) {
  buffer_start = cursor;
  buffer_end = cursor;
  while(tokens-- && buffer_end != text.end()) {
    ++buffer_end;
  }
}

// O(1)
void Editor::Cut(size_t tokens) {
  Copy(tokens);
  if(buffer_end != text.end()) {
    cursor = buffer_end;
    text.splice(text.end(),text,buffer_start,buffer_end);
  } else {
    cursor = buffer_start;
  }
  text_end = buffer_start;
  buffer_end = text.end();
}

// O(1)
void Editor::Paste() {
  if(cursor != buffer_start) {  
    text.splice(cursor,text,buffer_start,buffer_end);
  }
  cursor = buffer_end;
  text_end = text.end();
}

// O(N)
string Editor::GetText() const {
  return string(text.begin(),text_end);
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
    TypeText(editor, "abcde, 12345");
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
    
    ASSERT_EQUAL(editor.GetText(), "12345, abcde");
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