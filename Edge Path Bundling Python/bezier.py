import numpy as np
import math
def evalBezier(controlPoints, t): 
    #arguments: list of 2D-np.array, float
    #return list
    #Check if we have enough control points
    if len(controlPoints) < 2:
        retVal = np.array(0.0, 0.0)
        return retVal
    if (t <0 or t>1):
        retVal = np.array(0.0, 0.0)
        return retVal
    if (t==0):
        return controlPoints[0]
    if (t==1):
        return controlPoints[-1]

    #Calculate the intermediate points
    points = controlPoints
    while (len(points) > 1):
        intermediatePoints = []
        for i in range(len(points)-1):
            p = (1 - t) * points[i] + t * points[i+1]
            intermediatePoints.append(p)
        points = intermediatePoints
    return points[0]


def createBezierPolygon(controlPoints, n): #n = number of points to approximate curve
#arguments: list of 2d-np.arrays , int
#returns: list of 2d np.arrays (points) on the bezier curve
    if n < 2:
        return [controlPoints[0],controlPoints[-1]]
    points = []
    for i in range(n):
        points.append(evalBezier(controlPoints, i/n))
    points.append(controlPoints[-1])
    return points