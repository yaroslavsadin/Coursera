#include <iostream>
#include "yellow_pages.h"

int main(void) {
    std::cout << "Hello from Yellow Pages tests!" << std::endl;
    YP::Database db;
    db.AddCompany("lol");
    std::cout << "Succesfully added a company!" << std::endl;
    return 0;
}