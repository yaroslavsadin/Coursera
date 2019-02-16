#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

using namespace std;

// Реализуйте здесь
// * класс Matrix
// * оператор ввода для класса Matrix из потока istream
// * оператор вывода класса Matrix в поток ostream
// * оператор проверки на равенство двух объектов класса Matrix
// * оператор сложения двух объектов класса Matrix

class Matrix {
public:
  Matrix() {}
  Matrix(int num_rows, int num_cols) {
    Reset(num_rows, num_cols);
  }
  void Reset(int num_rows, int num_cols) {
    if(num_rows >= 0 && num_cols >= 0) {
      matrix.resize(num_rows);
      for(auto& m : matrix) {
        m.resize(num_cols);
      }
    }
    else
    {
        throw(out_of_range("Out of range, num_rows: "+to_string(num_rows)+
        ", num_cols: "+to_string(num_cols)));
    }
  }
  int At(int row, int col) const {
    if(row < GetNumRows() && col < GetNumColumns()) {
      return matrix[row][col];
    } else {
      throw(out_of_range("Out of range, row: "+to_string(row)+
        ", col: "+to_string(col)));
    }
  }
  int& At(int row, int col) {
    if(row < GetNumRows() && col < GetNumColumns()) {
      int& res = matrix[row][col];
      return res;
    } else {
      throw(out_of_range("Out of range, row: "+to_string(row)+
        ", col: "+to_string(col)));
    }
  }
  int GetNumRows() const {
    return matrix.size();
  }
  int GetNumColumns() const {
    if(matrix.size())  
      return matrix[0].size();
    else
      return 0;
  }
private:
  vector<vector<int>> matrix;
};

istream& operator>>(istream& stream, Matrix& matrix) {
  int num_rows,num_cols;
  stream >> num_rows >> num_cols;
  matrix.Reset(num_rows,num_cols);
  for(int i = 0; i < matrix.GetNumRows(); i++) {
    for(int j = 0; j < matrix.GetNumColumns(); j++) {
      stream >> matrix.At(i,j);
    }
  }
  return stream;
}

ostream& operator<<(ostream& stream, Matrix& matrix) {
  int num_rows = matrix.GetNumRows();
  int num_cols = matrix.GetNumColumns();
  stream << num_rows << " " << num_cols << endl;
  for(int i = 0; i < matrix.GetNumRows(); i++) {
    for(int j = 0; j < matrix.GetNumColumns(); j++) {
      stream << matrix.At(i,j) << " ";
    }
    stream << endl;
  }
  return stream;
}

bool operator==(const Matrix& lhs,const Matrix& rhs) {

}

bool operator+(const Matrix& lhs,const Matrix& rhs) {

}

int main() {
  Matrix one;
  Matrix two;
#if 0
  try {
    Matrix three{-4,5};
  } catch(out_of_range& e) {
    cout << e.what() << endl;
  }

  try {
    Matrix four{4,-5};
  } catch(out_of_range& e) {
    cout << e.what() << endl;
  }

  cout << two.At(1,2) << endl;
  try {  
    cout << two.At(4,6) << endl;
  } catch(out_of_range& e) {
    cout << e.what() << endl;
  }
  try {  
    cout << two.At(9,5) << endl;
  } catch(out_of_range& e) {
    cout << e.what() << endl;
  }

  int& a = two.At(1,1);
  a++;
  cout << two.At(1,1) << endl;

  cout << one.GetNumColumns() << " " << one.GetNumRows() << endl;
  cout << two.GetNumColumns() << " " << two.GetNumRows() << endl;
  one.Reset(4,4);
  cout << one.GetNumColumns() << " " << one.GetNumRows() << endl;

  int& b = one.At(2,3);
  b = 666;
  cout << one.At(2,3) << endl;
#endif
  cin >> one;
  cout << one << endl;
  return 0;
}
