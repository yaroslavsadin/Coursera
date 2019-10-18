#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <memory>
#include <algorithm>
#include "misc.h"
#include "distances.h"
#include "descriptions.h"

class BusDatabase {
public:
    void AddStop(std::string name, double latitude, double longtitude, 
                    std::vector< std::pair< std::string, unsigned int > >);
    void AddBus(const std::string& name, std::vector<std::string> stops, bool is_circular);

    std::optional<const Bus*>  GetBusInfo (const std::string& name) const;
    std::optional<const Stop*>  GetStopInfo (const std::string& name) const;

    const Buses& GetBuses() const { return buses_; }
    const Stops& GetStops() const { return stops_; }

    const Distances& GetBusDistance(const std::string& name) const;
private:
    Stops stops_;
    Buses buses_;
    
    Distances ComputeDistance(const Bus& bus) const;
    mutable std::unordered_map< std::string_view , Distances > bus_to_distance_cache;
};