// Example program
#include <iostream>
#include <string>
#include <set>
#include <map>
using namespace std;
int main() {
    set<set<string>> DIC;
    set<string> p;
    string command;
    string s1;
    string s2;
    int Q;
    cin >> Q;
    for(int i=0; i<Q; i++){
        cin >> command;
        if(command == "ADD"){
            p.clear();
            cin >> s1 >> s2;
            p.insert(s1);
            p.insert(s2);
            DIC.insert(p);
            }
        else if(command == "COUNT"){
            cin >> s1;
            int c = 0;
            for(const auto& item : DIC){
                for (const auto& it : item){
                    if (it == s1){
                        c++;
                        continue;
                        }
                    }
                }
            cout << c << endl;
            }
        else if(command == "CHECK") {
            p.clear();
            cin >> s1 >> s2;
            p.insert(s1);
            p.insert(s2);
            bool check = false;
            for(const auto& item : DIC) {
                if(item == p){
                    check = true;
                    //cout << "YES" << endl;
                    continue;
                }
            }
            if(check == true){
                cout << "YES" << endl;
                }
            else{
                cout << "NO" << endl;
                }
        }
    }
}