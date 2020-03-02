#include <iostream>
#include "database.pb.h"
#include "yp_index.h"

int main(void) {
    YellowPages::Database db;
    YP::YellowPagesIndex index(db);
    return 0;
}