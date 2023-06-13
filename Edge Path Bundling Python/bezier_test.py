import matplotlib.pyplot as plt
import bezier as bz
import numpy as np

n = 100
controlPoints = [np.array([0, 0]), np.array([1, 0]), np.array([1, 1]), np.array([0, 1])]
polygon = bz.createBezierPolygon(controlPoints, n) # returns list of 2d vectors

x = [arr[0] for arr in polygon]
y = [arr[1] for arr in polygon]

plt.plot(x, y)
plt.show()