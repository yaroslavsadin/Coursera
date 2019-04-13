#include <string>
#include <vector>
using namespace std;

// Реализуйте этот макрос так, чтобы функция main компилировалась
#define MAKE_AGAIN(line) id_##line
#define MAKE_UNIQUE(line) MAKE_AGAIN(line)
#define UNIQ_ID MAKE_UNIQUE(__LINE__)

int main() {
  int UNIQ_ID = 0;
  string UNIQ_ID = "hello";
  vector<string> UNIQ_ID = {"hello", "world"};
  vector<int> UNIQ_ID = {1, 2, 3, 4};
}