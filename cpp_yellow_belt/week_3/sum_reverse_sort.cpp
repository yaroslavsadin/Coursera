#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include "sum_reverse_sort.h"
#include "test_runner.h"
using namespace std;

int Sum(int x, int y) {
    return x + y;
}

string Reverse(string s) {
    for(size_t i = 0; i < s.size()/2; i++) {
       char t = s[i];
       s[i] = s[s.size()-i-1];
       s[s.size()-i-1] = t;
    }
    return s;
}

void Sort(vector<int>& nums) {
    sort(begin(nums),end(nums));
}

#if 1
void TestSum(void) {
    AssertEqual(4,Sum(2,2),"2+2 != 4",__LINE__,__FILE__);
    AssertEqual(4,Sum(6,-2),"6+(-2) != 4",__LINE__,__FILE__);
    AssertEqual(-4,Sum(-2,-2),"2+2 != 4",__LINE__,__FILE__);
}

void TestReverse(void) {
    {
        string expected{"lolkek"};
        AssertEqual(expected,Reverse("keklol"),"keklol",__LINE__,__FILE__);
    }
}

void TestSort(void) {
    {
        vector<int> expected = {1,2,3,4,5,6,7};
        vector<int> result = {5,3,2,4,7,1,6};
        Sort(result);
        AssertEqual(expected,result,"1,2,3,4,5,6,7",__LINE__,__FILE__);
    }
}

int main(void) {
    TestRunner t;
    t.RunTest(TestSort,"TestSort");
    t.RunTest(TestReverse,"TestReverse");
    t.RunTest(TestSum,"TestSum");
    return 0;
}
#endif