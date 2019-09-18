#include <iostream>
#include "test_runner.h"
#include "database.h"
#include "misc.h"
#include "io.h"

using namespace std;

int main(void) {    
    BusDatabaseHandler handler;
    int num;
    try {
        cin >> num;
        handler.RequestsFromStream(num,cin);
        cin >> num;
        auto responses = handler.RequestsFromStream(num,cin).ProcessRequests().GetResponses();
        for(const auto& resp : responses) {
            cout << resp << endl;
        }
    } catch(exception& e) {
        cerr << e.what() << endl;
    }
    return 0;
}