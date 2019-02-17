#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <sstream>

using namespace std;

// Реализуйте здесь
// * класс Matrix
// * оператор ввода для класса Matrix из потока istream
// * оператор вывода класса Matrix в поток ostream
// * оператор проверки на равенство двух объектов класса Matrix
// * оператор сложения двух объектов класса Matrix

class Matrix {
public:
  Matrix() {
    Reset(0, 0);
  }
  Matrix(int num_rows, int num_cols) {
    Reset(num_rows, num_cols);
  }
  void Reset(int num_rows, int num_cols) {
    // Can use single assign method on matrix...
    // elements_.assign(num_rows, vector<int>(num_columns));
    if(num_rows >= 0 && num_cols >= 0) {
      matrix.resize(num_rows);
      for(auto& m : matrix) {
        m.assign(num_cols,0);
      }
      columns = num_cols;
    }
    else
    {
        throw(out_of_range("Out of range, num_rows: "+to_string(num_rows)+
        ", num_cols: "+to_string(num_cols)));
    }
  }
  int At(int row, int col) const {
    // out_of_range could be thrown by the vecotr itself??
    // return elements_.at(row).at(column);
    if(row < GetNumRows() && col < GetNumColumns() &&
       row >= 0 && col >= 0) {
      return matrix[row][col];
    } else {
      throw(out_of_range("Out of range, row: "+to_string(row)+
        ", col: "+to_string(col)));
    }
  }
  int& At(int row, int col) {
    if(row < GetNumRows() && col < GetNumColumns() &&
       row >= 0 && col >= 0) {
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
    return columns;
  }
private:
  vector<vector<int>> matrix;
  int columns;
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

ostream& operator<<(ostream& stream, const Matrix& matrix) {
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
  if(rhs.GetNumRows() != lhs.GetNumRows() || rhs.GetNumColumns() != lhs.GetNumColumns()) {
    return false;
  } else {
    for(int i = 0; i < rhs.GetNumRows(); i++) {
      for(int j = 0; j < rhs.GetNumColumns(); j++) {
        if(lhs.At(i,j) != rhs.At(i,j)) {
          return false;
        }
      }
    }
    return true;
  }
}

Matrix operator+(const Matrix& lhs,const Matrix& rhs) {
  if(rhs.GetNumRows() != lhs.GetNumRows() || rhs.GetNumColumns() != lhs.GetNumColumns()) {
    throw(invalid_argument("Invalid argument: "+to_string(rhs.GetNumRows())+" "+to_string(lhs.GetNumRows())+" "
    +to_string(rhs.GetNumColumns())+" "+to_string(lhs.GetNumColumns())));
  } else {
    Matrix res{lhs.GetNumRows(), lhs.GetNumColumns()};
    for(int i = 0; i < rhs.GetNumRows(); i++) {
      for(int j = 0; j < rhs.GetNumColumns(); j++) {
        res.At(i,j) = lhs.At(i,j) + rhs.At(i,j);
      }
    }
    return res;
  }
}

int main() {
#if 1
  /************* Test constructors **************/
  try {
    Matrix matrix(0,0);
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  try {
    Matrix matrix(-1,0);
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  try {
    Matrix matrix(0,-1);
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  /******* Test GetNumRows GetNumColumns ********/
  try {
    Matrix m(9,7);
    cout << m.GetNumColumns() << m.GetNumRows();
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  /**************** Test .At() ******************/
  try {
    Matrix m(5,4);
    m.At(5,3);
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  try {
    Matrix m(5,4);
    m.At(4,4);
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  try {
    Matrix m(5,4);
    m.At(-1,1);
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  try {
    Matrix m(5,4);
    m.At(1,-1);
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  /*************** Test .Reset() ****************/
  try {
    Matrix m(4,4);
    m.Reset(0,0);
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  try {
    Matrix m(4,4);
    m.Reset(-5,0);
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  try {
    Matrix m(4,4);
    m.Reset(0,-3);
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  try {
    Matrix m(4,4);
    m.Reset(-3,-3);
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  /*************** Test operators ****************/
  try {
    Matrix m1(4,4);
    Matrix m2(4,4);
    stringstream ss{"2 4 1 2 3 4 5 6 7 8 2 4 8 7 6 5 4 3 2 1"};
    ss >> m1;
    ss >> m2;
    cout << (m1 + m2) << endl;
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  try {
    Matrix m1(4,4);
    Matrix m2(4,4);
    stringstream ss{"2 4 1 2 3 4 5 6 7 8 2 2 8 7 6 5"};
    ss >> m1;
    ss >> m2;
    cout << (m1 + m2) << endl;
  } catch(invalid_argument& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  try {
    Matrix m1;
    Matrix m2;
    stringstream ss{"3 3 6 4 -1 9 8 12 1 2 9 -5 -4 0 12 8 6 3 5 5 1 0 -8 23 14 5 -6 6 9 8 0 5 4 1"};
    ss >> m1;
    ss >> m2;
    cout << (m1 + m2) << endl;
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  try {
    Matrix m1(0,0);
    Matrix m2(1,0);
    cout << (m1 + m2) << endl;
  } catch(invalid_argument& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  try {
    Matrix m1(0,3);
    Matrix m2(0,5);
    cout << (m1 + m2) << endl;
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
  try {
    Matrix m1;
    Matrix m2;
    cout << (m1 + m2) << endl;
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
#else
  Matrix one;
  Matrix two;
  try {
    cin >> one;
    cin >> two;
    Matrix three = one + two;
    cout << three << endl;
  } catch(exception& e) {
    cout << e.what() << endl;
    cout << "Line: " << __LINE__ << endl;
  }
#endif
  return 0;
}
