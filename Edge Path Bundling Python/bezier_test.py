import matplotlib.pyplot as plt
import bezier as bz
import numpy as np
import bezierSpherical as bz3d
import bezier as bz

n = 50
controlPoints = [np.array([150,20]), np.array([-179,-60]), np.array([-150, 20]) ]

bez = bz3d.createSphericalBezierPolygon(controlPoints,-1)
xBez = [arr[0] for arr in bez]
yBez = [arr[1] for arr in bez]
plt.plot(xBez, yBez) #blue

geodesic = bz3d.createGeodesicPolygon(controlPoints[0], controlPoints[-1],-1)
xGeo = [arr[0] for arr in geodesic]
yGeo = [arr[1] for arr in geodesic]


plt.plot(xGeo, yGeo) #red
boundaries=[[-180,-90],[-180, 90],[180,90],[180,-90],[-180,-90] ]
x,y = zip(*boundaries)
plt.plot(x, y, color='black', linewidth=0.1,  alpha=1)





plt.show()