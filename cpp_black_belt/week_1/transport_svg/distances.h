#pragma once

struct Distances {
    unsigned int road_distance;
    double linear_distance;
};

double toRad(double degree);
double CalcDistance(double latitude_from, double longitude_from, 
                    double latitude_to, double longitude_to);