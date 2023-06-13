import numpy as np
import math
import pandas as pd
import typing
from typing import List
from tqdm import tqdm
import geopandas as gpd
import descartes
import matplotlib.pyplot as plt
import heapq

import bezier as bz
import airports
import migrations
import control_points
from model import Edge, Node
def toSphere(v):
        phi = v[0] # longitude
        theta = (np.pi/2) - v[1] # latitude
        x = np.sin(theta) * np.cos(phi)
        y = np.sin(theta) * np.sin(phi)
        z = np.cos(theta)
        return np.array([x,y,z])

def toPlane(v):
    r = np.linalg.norm(v)
    r2D = np.sqrt(np.square(v[0]) + np.square(v[1]))
    theta = np.arccos(v[2]/r)
    lat = (np.pi/2)-theta
    lon = np.sign(v[1]) * np.arccos(v[0]/r2D)
    return(np.array([lon,lat]))

def rotate(vector, axis, theta):
    """
    Return the rotation matrix associated with counterclockwise rotation about
    the given axis by theta radians.
    """
    axis = np.asarray(axis)
    axis = axis / math.sqrt(np.dot(axis, axis))
    a = math.cos(theta / 2.0)
    b, c, d = -axis * math.sin(theta / 2.0)
    aa, bb, cc, dd = a * a, b * b, c * c, d * d
    bc, ad, ac, ab, bd, cd = b * c, a * d, a * c, a * b, b * d, c * d
    mat = np.array([[aa + bb - cc - dd, 2 * (bc + ad), 2 * (bd - ac)],
                     [2 * (bc - ad), aa + cc - bb - dd, 2 * (cd + ab)],
                     [2 * (bd + ac), 2 * (cd - ab), aa + dd - bb - cc]])
    return (np.dot(mat, vector))


def evalBezierSpherical(controlPoints, t):
#arguments:  list of 2D-np.arrrays, int
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
    points3D = []
    for point in controlPoints:
        points3D.append(toSphere(point))
    while (len(points3D) > 1):
        intermediatePoints = []
        for i in range(len(points3D)-1):
            unit1 = points3D[i] / np.linalg.norm(points3D[i])
            unit2 = points3D[i+1] / np.linalg.norm(points3D[i+1])
            cosAlpha = np.dot(unit1,unit2)
            alpha = np.arccos(cosAlpha)
            axis = np.cross(unit1, unit2)
            v = rotate(points3D[i], axis, t*alpha)
            intermediatePoints.append(v)
        points3D = intermediatePoints
    return toPlane(points3D[0])


def createGeodesicPolygon(start, end, n):# startPoint, endPoint, numberOfIntermediateSteps
#arguments: 2d np.array , 2d np.array, int
#returns: 2d np.array
    if((start==end).all()):
        return start
    a = toSphere(start)
    b = toSphere(end)
    unit1 = a / np.linalg.norm(a)
    unit2 = b / np.linalg.norm(b)
    cosAlpha = np.dot(unit1,unit2)
    alpha = np.arccos(cosAlpha)  
    axis = np.cross(unit1, unit2)
    samplesOnSphere = []
    for i in range(n):
        v = rotate(a, axis, i*alpha/n)
        samplesOnSphere.append(v)
    samplesOnSphere.append(b)
    samplesOnMap = []
    for sample3D in samplesOnSphere:
        print([sample3D,toPlane(sample3D)])
        samplesOnMap.append(toPlane(sample3D))
    return samplesOnMap



def createSphericalBezierPolygon(controlPoints, n): #n = number of points to approximate curve
#arguments: list of 2d-np.arrays , int
#returns: list of 2d np.arrays (points) on the bezier curve w.r.t spherical geometry
    if n < 2:
        return [controlPoints[0],controlPoints[-1]]
    points = []
    for i in range(n):
        points.append(evalBezierSpherical(controlPoints, i/n))
    points.append(controlPoints[-1])
    return points



def plotSpherical(controlPointLists, nodes, edges,n):
    # create and bezier curves
    bezierPolygons = []
    for controlPoints in tqdm(controlPointLists, desc="Drawing: "):
        polygon = bz.create_bezier_polygon(controlPoints, n)  # returns list of 2d vectors
        bezierPolygons.append(polygon)
        x = [arr[0] for arr in polygon]
        y = [arr[1] for arr in polygon]
        plt.plot(x, y, color='red', linewidth=0.1, alpha=1)

    # draw lines without detour or with detour that was too long
    for edge in edges:
        if edge.skip:
            continue

        s = nodes[edge.source]
        d = nodes[edge.destination]
        x = [s.longitude, d.longitude]
        y = [s.latitude, d.latitude]
        plt.plot(x, y, color='red', linewidth=0.1,  alpha=1)

    for node in nodes.values():
        a = (node.longitude, node.latitude)
        c = plt.Circle(a, radius=0.1, color='green')
        #ax.add_patch(c)

    plt.axis('scaled')
    plt.axis('off')
    plt.gcf().set_dpi(300)
    plt.tight_layout()
    plt.show()

def test():
    a = np.array([0.5* np.pi, 0.1*np.pi])
    print(a)
    b = toSphere(a)
    print(b)
    n = np.array([0,0,1])
    alpha=np.pi
    c = rotate(b,n,alpha)
    print(c)
    d = toPlane(c)
    print(d)