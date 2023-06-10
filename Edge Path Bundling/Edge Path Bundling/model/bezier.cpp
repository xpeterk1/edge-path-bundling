#pragma once
#include "bezier.h"

Coordinates bezierFlat(const std::vector<Coordinates>& controlPoints, float t) 
{
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

glm::vec3 bezierSphere(const std::vector<Coordinates>& controlPoints, float t, float radius)
{
    // Check if we have enough control points
    if (controlPoints.size() < 2)
    {
        std::cerr << "\nError: Less than 2 ctrl points" << std::endl;
        return glm::vec3(0,0,0);
    }
    if (t < 0 or t>1)
    {
        std::cerr << "\nError: t not in [0,1]" << std::endl;
        return glm::vec3(0, 0, 0);
    }
    
    // covert long/lat control points to 3d vectors
    std::vector<glm::vec3> points;
    for(Coordinates point : controlPoints)
    {
        float phi = point.longitude;
        float theta = 90.0f - point.latitude;
        glm::vec3 currentVector(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), std::cos(theta));
        points.push_back(radius * currentVector);
    }

    // Calculate the intermediate points
    while(points.size() > 1)
    {
        std::vector<glm::vec3> intermediatePoints;
        for (size_t i = 0; i < points.size() - 1; ++i) 
        {
            glm::vec3 vector1 = points[i];
            glm::vec3 vector2 = points[i + 1];
            glm::vec3 n = glm::cross(vector1, vector2);
            float cosAlpha = glm::dot(vector1, vector2);
            float alphaRad = glm::acos(cosAlpha);
            float betaRad= t * alphaRad; 
            glm::mat4 rotationMat = glm::rotate(betaRad, n);
            glm::vec4 result4d = rotationMat * glm::vec4(vector1,1);
            glm::vec3 result(result4d.x, result4d.y, result4d.z);
            intermediatePoints.push_back(result);
        }
        points = std::move(intermediatePoints);
    }
    return points[0];

}