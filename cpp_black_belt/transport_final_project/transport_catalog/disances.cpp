#include "distances.h"
#include <cmath>

double toRad(double degree) {
    constexpr double PI = 3.1415926535;
    return degree/180 * PI;
}

double CalcDistance(double latitude_from, double longitude_from, 
                    double latitude_to, double longitude_to) {
    double dist;
    dist = sin(toRad(latitude_from)) * sin(toRad(latitude_to)) + cos(toRad(latitude_from)) 
            * cos(toRad(latitude_to)) * cos(toRad(longitude_from - longitude_to));
    dist = acos(dist);
    //got dist in radian, no need to change back to degree and convert to rad again.
    dist = 6371 * 1000 * dist;
    return dist;
}