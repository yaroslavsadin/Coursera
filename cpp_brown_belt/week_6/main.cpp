#include <iostream>
#include "test_runner.h"
#include "database.h"
#include "misc.h"
#include "io.h"

using namespace std;

int main(void) {
    string_view bus_req_test_str("Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka");
    AddBusRequest bus_request_test(bus_req_test_str);
    return 0;
}