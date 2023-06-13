import matplotlib.pyplot as plt
import bezier as bz
import numpy as np
import bezierSpherical as bz3d
import bezier as bz

n = 50
controlPoints = [np.array([0,0.25*np.pi]), np.array([0.5*np.pi, 0.25*np.pi]), np.array([np.pi,0.25*np.pi]) ]
polygonSphere = bz3d.createSphericalBezierPolygon(controlPoints, n)
polygonFlat = bz.create_bezier_polygon(controlPoints, n)


x1 = [arr[0] for arr in polygonFlat]
y1 = [arr[1] for arr in polygonFlat]

x2 = [arr[0] for arr in polygonSphere]
y2 = [arr[1] for arr in polygonSphere]

plt.plot(x1, y1)
plt.plot(x2, y2)
plt.show()