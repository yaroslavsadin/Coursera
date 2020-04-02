#pragma once
#include <iostream>
#include "json.h"
#include "time_interval.h"
#include "database.pb.h"

namespace YP {
    void Serialize(const Json::Document& doc, YellowPages::Database& db);
}