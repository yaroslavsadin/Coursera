#include "address.pb.h"
#include "person.pb.h"
#include <iostream>
#include <fstream>

int main(void) {
    {   
        // Serialize::Address addr;
        // addr.set_building(16);
        // addr.set_street("Lva ");
        // *addr.mutable_street() += "Tolstogo";
        // std::cout << addr.street() << ' ' << addr.building() << '\n';

        // Serialize::Person p;
        // p.add_phone_number("123");
        // p.add_phone_number("321");

        // for(const std::string& phone : p.phone_number()) {
        //     std::cout << phone << ' ';
        // }
        // std::cout << std::endl;

        // *p.mutable_address() = addr;
        // std::fstream out("person.bin", std::ios::binary | std::ios::out);
        // p.SerializeToOstream(&out);
    }
    {
        std::fstream in("person.bin", std::ios::binary | std::ios::in);
        Serialize::Person p;

        if(p.ParseFromIstream(&in)) {
            std::cout << "Name " << p.name() << '\n';
            std::cout << "Phone number count " << p.phone_number_size() << '\n';
            if(p.has_address()) {
                std::cout << p.address().street() << '\n';
            } else {
                std::cout << "No address\n";
            }
        } else {
            std::cout << "Failed to parse message\n";
        }
    }

    return 0;
}