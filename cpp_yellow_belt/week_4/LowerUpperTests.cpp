#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include "LowerUpperTests.h"
#include "LowerUpper.h"
#include "test_runner.h"
using namespace std;

void TestLowerBound(void) {
    {
        vector<string> input {"a","b","d","e"};
        string expected = "a";
        string result = *LowerBound(input.begin(),input.end(),'a'-1);
        AssertEqual(result,expected,"Less than first",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a","b","d","e"};
        auto expected = input.end() - input.begin();
        auto result = LowerBound(input.begin(),input.end(),'e'+1) - input.begin();
        AssertEqual(result,expected,"More than last",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a","b","d","e"};
        string expected = "a";
        string result = *LowerBound(input.begin(),input.end(),'a');
        AssertEqual(result,expected,"Just 'a'",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a","b","d","e"};
        string expected = "d";
        string result = *LowerBound(input.begin(),input.end(),'d');
        AssertEqual(result,expected,"Just 'd'",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a","b","d","e"};
        string expected = "d"; // correct???
        string result = *LowerBound(input.begin(),input.end(),'c');
        AssertEqual(result,expected,"Between 'a' and 'b - 'c'",__LINE__,__FILE__);
    }
    {
        vector<string> input {};
        int expected = input.end() - input.begin();
        auto result = UpperBound(input.begin(),input.end(),static_cast<char>('z')) - input.begin();
        AssertEqual(result,expected,"Empty vector",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a"};
        int expected = input.end() - input.begin();
        auto result = UpperBound(input.begin(),input.end(),'b') - input.begin();
        AssertEqual(result,expected,"1-elem vector",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a"};
        string expected = "a";
        string result = *LowerBound(input.begin(),input.end(),'a');
        AssertEqual(result,expected,"1-elem vector",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a"};
        string expected = "a";
        string result = *LowerBound(input.begin(),input.end(),static_cast<char>('a'-1));
        AssertEqual(result,expected,"1-elem vector",__LINE__,__FILE__);
    }
}

void TestUpperBound(void) {
    {
        vector<string> input {"a","b","d","e"};
        string expected = "a";
        string result = *UpperBound(input.begin(),input.end(),'a'-1);
        AssertEqual(result,expected,"Less than first",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a","b","d","e"};
        auto expected = input.end() - input.begin();
        auto result = UpperBound(input.begin(),input.end(),'e'+1) - input.begin();
        AssertEqual(result,expected,"More than last",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a","b","d","e"};
        string expected = "b";
        string result = *UpperBound(input.begin(),input.end(),'a');
        AssertEqual(result,expected,"Just 'a'",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a","b","d","e"};
        string expected = "e";
        string result = *UpperBound(input.begin(),input.end(),'d');
        AssertEqual(result,expected,"Just 'd'",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a","b","d","e"};
        string expected = "d"; // correct???
        string result = *UpperBound(input.begin(),input.end(),'c');
        AssertEqual(result,expected,"Between 'a' and 'b - 'c'",__LINE__,__FILE__);
    }
    {
        vector<string> input {};
        int expected = input.end() - input.begin();
        auto result = UpperBound(input.begin(),input.end(),static_cast<char>('z')) - input.begin();
        AssertEqual(result,expected,"Empty vector",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a"};
        int expected = input.end() - input.begin();
        auto result = UpperBound(input.begin(),input.end(),'b') - input.begin();
        AssertEqual(result,expected,"1-elem vector",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a"};
        int expected = input.end() - input.begin();
        auto result = UpperBound(input.begin(),input.end(),'a') - input.begin();
        AssertEqual(result,expected,"1-elem vector",__LINE__,__FILE__);
    }
    {
        vector<string> input {"a"};
        string expected = "a";
        string result = *UpperBound(input.begin(),input.end(),static_cast<char>('a'-1));
        AssertEqual(result,expected,"1-elem vector",__LINE__,__FILE__);
    }
}