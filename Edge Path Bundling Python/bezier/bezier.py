import numpy as np
import math
from numba import jit


# arguments: list of 2D-np.array, float
# return list
# Check if we have enough control points
@jit(nopython=True)
def eval_bezier(control_points, t):

    if len(control_points) < 2:
        return np.zeros(2)

    if t < 0 or t > 1:
        return np.zeros(2)
    if t == 0:
        return control_points[0]
    if t == 1:
        return control_points[-1]

    # Calculate the intermediate points
    points = control_points
    while len(points) > 1:
        intermediate_points = []
        for i in range(len(points)-1):
            p = (1 - t) * points[i] + t * points[i+1]
            intermediate_points.append(p)
        points = intermediate_points
    return points[0]


# n = number of points to approximate curve
# arguments: list of 2d-np.arrays , int
# returns: list of 2d np.arrays (points) on the bezier curve
@jit(nopython=True)
def create_bezier_polygon(control_points, n):

    if n < 2:
        return [control_points[0], control_points[-1]]
    points = []
    for i in range(n):
        points.append(eval_bezier(control_points, i/n))
    points.append(control_points[-1])
    return points
