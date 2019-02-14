#include <iostream>
#include <string>
using namespace std;

int main()
{
   int m = 0;
   int n = 0;
   auto ans = [&, n] (int a) mutable -> string { 
       m = ++n + a; 
       if (m > 5) return "lol"; 
       else return "kek"; 
    }(4);
   cout << m << endl << n << endl;
   cout << ans << endl;
}