#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include <sstream>
using namespace std;

template <typename T> vector<T> operator * (const vector<T>& first, const vector<T>& second);
template <typename Key, typename Value> map<Key,Value> operator * (const map<Key,Value>& first, const map<Key,Value>& second);
template <class T1, class T2> pair<T1,T2> operator * (const pair<T1,T2>& first, const pair<T1,T2>& second);
template<class T> T Sqr(T x);
#if 1
template <typename Collection> string Join(const Collection& c, char d);
template <typename First, typename Second> ostream& operator << (ostream& out, const pair<First, Second>& p);
template <typename T> ostream& operator << (ostream& out, const vector<T>& vi);
template <typename Key, typename Value> ostream& operator << (ostream& out, const map<Key, Value>& m);
#endif

template <typename T>
vector<T> operator * (const vector<T>& first, const vector<T>& second) {
// Assuming vectors are of the same length
  vector<T> res(first.size());
  for(int i = 0; i < res.size(); i++) {
      res[i] = first[i] * second[i];
  }
  return res;
}

template <typename Key, typename Value>
map<Key,Value> operator * (const map<Key,Value>& first, const map<Key,Value>& second) {
// Assuming maps have equvalent keys
  map<Key,Value> res;
  for(const auto& value : first) {
      res[value.first] = value.second * second.at(value.first);
  }
  return res;
}

template <class T1, class T2>
pair<T1,T2> operator * (const pair<T1,T2>& first, const pair<T1,T2>& second) {
    return {(first.first) * (second.first), (first.second) * (second.second)};
}

template<class T>
T Sqr(T x) {
    return x * x;
}
#if 1
template <typename Collection>
string Join(const Collection& c, char d) {
  stringstream ss;
  bool first = true;
  for (const auto& i : c) {
    if (!first) {
      ss << d;
    }
    first = false;
    ss << i;
  }
  return ss.str();
}

template <typename First, typename Second>
ostream& operator << (ostream& out, const pair<First, Second>& p) {
  return out << '(' << p.first << ',' << p.second << ')';
}

template <typename T>
ostream& operator << (ostream& out, const vector<T>& vi) {
  return out << '[' << Join(vi, ',') << ']';
}

template <typename Key, typename Value>
ostream& operator << (ostream& out, const map<Key, Value>& m) {
  return out << '{' << Join(m, ',') << '}';
}
#endif

int main(void) {
    // Пример вызова функции
    vector<int> v = {1, 2, 3};
    cout << "vector:";
    for (int x : Sqr(v)) {
        cout << ' ' << x;
    }
    cout << endl;

    map<int, pair<int, int>> map_of_pairs = {
    {4, {2, 2}},
    {7, {4, 3}}
    };
    cout << "map of pairs:" << endl;
    for (const auto& x : Sqr(map_of_pairs)) {
        cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
    }

    vector<map<int,pair<int,int>>> test = {
        {
            {1, {2,3}},
            {2, {9,2}},
            {3, {5,6}}
        },
        {
            {9, {7,4}},
            {4, {2,3}},
            {2, {5,2}}
        }
    };
    cout << Sqr(test) << endl;
    return 0;
}