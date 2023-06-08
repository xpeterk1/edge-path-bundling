#pragma once
#include <iostream>
#include <vector>

struct Coordinates 
{
    double longitude;
    double latitude;
};

//Returns 2d coordinates in the plane of a point on a bezier curve specified by t in [0,1]
Coordinates bezierFlat(const std::vector<Coordinates>& controlPoints, double t);

// TODO: write method that calculates bezier curve on sphere