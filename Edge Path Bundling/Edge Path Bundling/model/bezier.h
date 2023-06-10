#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include "glm/glm.hpp" 
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/constants.hpp"


struct Coordinates 
{
    double longitude;
    double latitude;
};


//Returns 2d coordinates in the plane of a point on a bezier curve specified by t in [0,1]
Coordinates bezierFlat(const std::vector<Coordinates>& controlPoints, float t);

// TODO: write method that calculates bezier curve on sphere
glm::vec3 bezierSphere(const std::vector<Coordinates>& controlPoints, float t, float radius = 1);