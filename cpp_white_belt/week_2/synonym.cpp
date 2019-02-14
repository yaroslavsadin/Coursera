#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

int main(void) {
    map<string,set<string>> synonyms;

    int cmd_cnt;
    cin >> cmd_cnt;

    for(int i = cmd_cnt; i > 0; i--) {
        string opcode;
        cin >> opcode;

        if(opcode == "ADD") {
            string a,b;
            cin >> a >> b;
            synonyms[a].insert(b);
            synonyms[b].insert(a);
        } else if(opcode == "COUNT") {
            string a;
            cin >> a;
            if(!synonyms.count(a)) {
                cout << 0 << endl;
            } else {
                cout << synonyms[a].size() << endl;
            }
        } else if(opcode == "CHECK") {
            string a,b;
            string ans {"NO"};
            cin >> a >> b;
            if(synonyms.count(a)) {
                if(synonyms[a].count(b)) {
                    cout << "YES" << endl;
                    continue;
                }
            }

            if(synonyms.count(b)) {
                if(synonyms[b].count(a)) {
                    cout << "YES" << endl;
                    continue;
                }
            }

            cout << "NO" << endl;
        }
    }
    return 0;
}