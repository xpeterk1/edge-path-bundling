#pragma once
#include "bezier.h"

Coordinates bezierFlat(const std::vector<Coordinates>& controlPoints, double t) {
    // Check if we have enough control points
    if (controlPoints.size() < 2)
    {
        std::cerr << "\nError: Less than 2 ctrl points" << std::endl;
        return Coordinates{ 0.0, 0.0 };
    }
    if (t <0 or t>1)
    {
        std::cerr << "\nError: t not in [0,1]" << std::endl;
        return Coordinates{ 0.0, 0.0 };
    }

    // Calculate the intermediate points
    std::vector<Coordinates> points = controlPoints;
    while (points.size() > 1)
    {
        std::vector<Coordinates> intermediatePoints;
        for (size_t i = 0; i < points.size() - 1; ++i) {
            Coordinates p{
                (1 - t) * points[i].longitude + t * points[i + 1].longitude,
                (1 - t) * points[i].latitude + t * points[i + 1].latitude
            };
            intermediatePoints.push_back(p);
        }
        points = std::move(intermediatePoints);
    }
    return points[0];
}