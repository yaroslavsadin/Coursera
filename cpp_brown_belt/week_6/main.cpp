#include <iostream>
#include "test_runner.h"
#include "misc.h"
#include "io.h"

using namespace std;

int main(void) {
#if 1
    BusDatabaseHandler handler;
    int num;
    try {
        cin >> num;
        handler.RequestsFromStream(num,cin);
        cin >> num;
        auto responses = handler.RequestsFromStream(num,cin).ProcessRequests().GetResponses();
        for(const auto& resp : responses) {
            cout << resp << '\n';
        }
    } catch(exception& e) {
        cerr << e.what() << endl;
    }
#else
    AddStopRequest add_stop_request("Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya");
#endif
    return 0;
}