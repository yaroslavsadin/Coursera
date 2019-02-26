#include "bus_manager.h"

void BusManager::AddBus(const string &bus, const vector<string> &stops)
{
    // Реализуйте этот метод
    buses_to_stops[bus] = stops;
    for (const string &stop : stops)
    {
        stops_to_buses[stop].push_back(bus);
    }
}

BusesForStopResponse BusManager::GetBusesForStop(const string &stop) const
{
    // Реализуйте этот метод
    stringstream ss;
    if (stops_to_buses.count(stop) == 0)
    {
        ss << "No stop";
    }
    else
    {
        for (const string &bus : stops_to_buses.at(stop))
        {
            ss << bus << " ";
        }
    }
    return {ss.str()};
}

StopsForBusResponse BusManager::GetStopsForBus(const string &bus) const
{
    // Реализуйте этот метод
    StopsForBusResponse r;
    stringstream ss;
    if (buses_to_stops.count(bus) == 0)
    {
        r.stopsForBusResponse.push_back("No bus");
    }
    else
    {
        for (const string &stop : buses_to_stops.at(bus))
        {
            ss << "Stop " << stop << ": ";
            if (stops_to_buses.at(stop).size() == 1)
            {
                ss << "no interchange";
            }
            else
            {
                for (const string &other_bus : stops_to_buses.at(stop))
                {
                    if (bus != other_bus)
                    {
                        ss << other_bus << " ";
                    }
                }
            }
            r.stopsForBusResponse.push_back(ss.str());
            ss.str("");
        }
    }
    return r;
}

AllBusesResponse BusManager::GetAllBuses() const
{
    // Реализуйте этот метод
    AllBusesResponse r;
    stringstream ss;
    if (buses_to_stops.empty())
    {
        r.allBusesResponse.push_back("No buses");
    }
    else
    {
        for (const auto &bus_item : buses_to_stops)
        {
            ss << "Bus " << bus_item.first << ": ";
            for (const string &stop : bus_item.second)
            {
                ss << stop << " ";
            }
            r.allBusesResponse.push_back(ss.str());
            ss.str("");
        }
    }
    return r;
}
