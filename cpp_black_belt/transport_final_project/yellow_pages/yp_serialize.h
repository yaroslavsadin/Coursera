#pragma once
#include <iostream>
#include "json.h"
#include "database.pb.h"

namespace YP {
    void Serialize(const Json::Document& doc, std::ostream& stream);
}