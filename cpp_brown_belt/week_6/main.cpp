#include <iostream>
#include "test_runner.h"
#include "database.h"
#include "misc.h"
#include "io.h"

using namespace std;

int main(void) {
    istringstream is {R"(
10
Stop Tolstopaltsevo: 55.611087, 37.20829
Stop Marushkino: 55.595884, 37.209755
Stop Rasskazovka: 55.632761, 37.333324
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
Stop Biryusinka: 55.581065, 37.64839
Stop Universam: 55.587655, 37.645687
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
3
Bus 256
Bus 750
Bus 751

    )"
    };
    
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