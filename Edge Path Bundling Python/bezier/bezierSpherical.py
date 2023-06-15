import numpy as np
import math
from tqdm import tqdm
import matplotlib.pyplot as plt
from numba import jit

from numba.core.errors import NumbaDeprecationWarning, NumbaPendingDeprecationWarning
import warnings

warnings.simplefilter('ignore', category=NumbaDeprecationWarning)
warnings.simplefilter('ignore', category=NumbaPendingDeprecationWarning)


@jit(nopython=True)
def to_sphere(v):
    phi = v[0]  # longitude
    theta = (np.pi/2) - v[1]  # latitude
    x = np.sin(theta) * np.cos(phi)
    y = np.sin(theta) * np.sin(phi)
    z = np.cos(theta)
    return np.array([x, y, z])


@jit(nopython=True)
def to_plane(v):
    r = np.linalg.norm(v)
    r2d = np.sqrt(np.square(v[0]) + np.square(v[1]))
    theta = np.arccos(v[2]/r)
    lat = (np.pi/2)-theta
    lon = np.sign(v[1]) * np.arccos(v[0]/r2d)
    return np.array([lon, lat])


@jit(nopython=True)
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
    return np.dot(mat, vector)


# arguments:  list of 2D-np.arrrays, int
@jit(nopython=True)
def eval_bezier_spherical(control_points, t):

    # Check if we have enough control points
    if len(control_points) < 2:
        return np.zeros(2)
    if t < 0 or t > 1:
        return np.zeros(2)
    if t == 0:
        return control_points[0]
    if t == 1:
        return control_points[-1]

    # Calculate the intermediate points
    points3d = [to_sphere(np.radians(point)) for point in control_points]

    while len(points3d) > 1:
        intermediate_points = []
        for i in range(len(points3d)-1):
            unit1 = points3d[i] / np.linalg.norm(points3d[i])
            unit2 = points3d[i+1] / np.linalg.norm(points3d[i+1])
            cos_alpha = np.dot(unit1, unit2)
            alpha = np.arccos(cos_alpha)
            axis = np.array([0, 0, 1])
            if alpha > 179.9:
                axis = np.cross(unit1, np.array([0, 0, 1]))
            else: 
                axis = np.cross(unit1, unit2)
            axis = axis/np.linalg.norm(axis)
            v = rotate(points3d[i], axis, t*alpha)
            intermediate_points.append(v)
        points3d = intermediate_points
    return np.degrees(to_plane(points3d[0]))


# startPoint, endPoint, numberOfIntermediateSteps, stepsize
# arguments: 2d np.array , 2d np.array, int
# returns: 2d np.array
def create_geodesic_polygon(start, end, n=0, step_size=1):
    if (start == end).all():
        return start
    
    # calculate resolution
    num_of_steps = n
    if n <= 0:
        delta = np.abs(end-start)
        if delta[0] > 180:
            delta[0] = delta[0]-180
        num_of_steps = np.ceil(np.linalg.norm(delta) / np.abs(step_size)).astype(int)

    # calculate Samples
    a = to_sphere(np.radians(start))
    b = to_sphere(np.radians(end))
    unit1 = a / np.linalg.norm(a)
    unit2 = b / np.linalg.norm(b)
    cos_alpha = np.dot(unit1, unit2)
    alpha = np.arccos(cos_alpha)
    axis = np.array([0, 0, 1])
    if alpha > 179.9:
        axis = np.cross(unit1, np.array([0,0,1]))
        axis = axis/axis(np.linalg.norm(axis))
    else: 
        axis = np.cross(unit1, unit2)
        axis = axis/np.linalg.norm(axis)
    samples_on_sphere = []

    for i in range(num_of_steps):
        v = rotate(a, axis, i*alpha/num_of_steps)
        samples_on_sphere.append(v)
    samples_on_sphere.append(b)
    return [np.degrees(to_plane(sample)) for sample in samples_on_sphere]
# end of function


# n = number of points to approximate curve
# arguments: list of 2d-np.arrays , int, float
# returns: list of 2d np.arrays (points) on the bezier curve w.r.t spherical geometry
def create_spherical_bezier_polygon(control_points, n=0, step_size=1):

    if n == 1:
        return [control_points[0], control_points[-1]]
    points = []
    
    # calculate resolution
    num_of_steps = n
    if n <= 0:
        length = 0  # approximates curve length
        for i in range(len(control_points) - 1):
            delta = control_points[i + 1] - control_points[i]
            if delta[0] > 180:
                delta[0] = delta[0]-180
            length = length + np.linalg.norm(delta)
        num_of_steps = np.ceil(length / np.abs(step_size)).astype(int)

    # create sample points
    for i in range(num_of_steps):
        points.append(eval_bezier_spherical(control_points, i / num_of_steps))
    points.append(control_points[-1])
    return points
# end of function


def plot_spherical(control_point_lists, nodes, edges, n=-1, step_size=1):
    # create and plot bezier curves
    for controlPoints in tqdm(control_point_lists, desc="Drawing 3D curves: "):
        polygon = create_spherical_bezier_polygon(controlPoints, n, step_size)  # returns list of 2d vectors
        
        # split polygon into sections when it crosses 180deg boundary
        split_indices = []
        skip = False
        for i in range(len(polygon)-1):
            a=polygon[i]
            d=polygon[i+1]
            if np.sign(a[0]) != np.sign(d[0]) and np.abs(a[0]) > 170 and np.abs(d[0]) > 170 and not skip:
                # create intermediate points close to border
                b = a
                b[0] = np.sign(a[0])*179.9
                b[1] = 0.5 * (a[1]+d[1])
                c=d
                c[0] = np.sign(d[0])*179.9
                c[1] = b[1]
                polygon.insert(i+1, b)
                polygon.insert(i+2, c)
                split_indices.append(i+2)
                skip = True
            else: 
                skip = False   
        segments = np.split(polygon, split_indices)

        for segment in segments:
            x = [arr[0] for arr in segment.tolist()]
            y = [arr[1] for arr in segment.tolist()]
            plt.plot(x, y, color='red', linewidth=0.1, alpha=1)

    # draw lines without detour or with detour that was too long
    for edge in tqdm(edges, desc="Drawing 3D lines"):
        if edge.skip:
            continue
        s = nodes[edge.source]
        d = nodes[edge.destination]
        start = np.array([s.longitude, s.latitude])
        end = np.array([d.longitude, d.latitude])
        polygon = create_geodesic_polygon(start, end, n, step_size)

        # split polygon into sections when it corosse 180deg boundary
        split_indices = []
        skip = False
        for i in range(len(polygon)-1):
            a = polygon[i]
            d = polygon[i+1]
            if np.sign(a[0]) != np.sign(d[0]) and np.abs(a[0]) > 170 and np.abs(d[0]) > 170 and not skip:
                # create intermediate points close to border
                b = a
                b[0] = np.sign(a[0])*179.9
                b[1] = 0.5 * (a[1]+d[1])
                c=d
                c[0] = np.sign(d[0])*179.9
                c[1] = b[1]
                polygon.insert(i+1, b)
                polygon.insert(i+2, c)
                split_indices.append(i+2)
                skip = True
            else: 
                skip = False   
        segments = np.split(polygon, split_indices)


        for segment in segments:
            x = [arr[0] for arr in segment.tolist()]
            y = [arr[1] for arr in segment.tolist()]
            plt.plot(x, y, color='blue', linewidth=0.1,  alpha=1)

    # draw boundaries of texture
    boundaries = [[-180, -90], [-180, 90], [180, 90], [180, -90], [-180, -90]]
    x, y = zip(*boundaries)
    plt.plot(x, y, color='black', linewidth=0.1,  alpha=1)

    # plt.axis('scaled')
    ax = plt.gca()
    ax.set_aspect('equal', adjustable='box')
    plt.axis('On')
    #plt.gcf().set_dpi(300)
    plt.tight_layout()

    # crop and save as texture png
    plt.savefig('spherical_texture.png')
    import cv2
    img = cv2.imread('spherical_texture.png')
    img = img[127:2348, 290:4735]
    cv2.imwrite("spherical_texture.png",img)
    plt.show()
# end of function