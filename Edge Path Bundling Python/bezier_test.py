import matplotlib.pyplot as plt
import bezier as bz
import numpy as np
import bezierSpherical as bz3d
import bezier as bz

n = 50
controlPoints = [np.array([20,0]), np.array([-179,-60]), np.array([-20, 0]) ]

polygon = bz3d.create_spherical_bezier_polygon(controlPoints, 200)
#split polygon into sections when it crosses 180deg boundary
splitIndices=[]
skip=False
for i in range(len(polygon)-1):
    a=polygon[i]
    d=polygon[i+1]
    if np.sign(a[0]) != np.sign(d[0]) and not skip:
        splitIndices.append(i+1)
        skip = True
    else: 
        skip = False   
segments = np.split(polygon, splitIndices) 
for segment in segments:
    x = [arr[0] for arr in segment.tolist()]
    y = [arr[1] for arr in segment.tolist()]
    plt.plot(x, y, color='blue', linewidth=0.1, alpha=1)

polygon = bz3d.create_geodesic_polygon(controlPoints[0], controlPoints[-1], 200)
#split polygon into sections when it crosses 180deg boundary
splitIndices=[]
skip=False
for i in range(len(polygon)-1):
    a=polygon[i]
    d=polygon[i+1]
    if np.sign(a[0]) != np.sign(d[0]) and not skip:
        #create intermediate points close to border
        b = a
        b[0] = np.sign(a[0])*179.9
        b[1] = 0.5 * (a[1]+d[1])
        c=d
        c[0] = np.sign(d[0])*179.9
        c[1] = b[1]
        polygon.insert(i+1, b)
        polygon.insert(i+2, c)
        splitIndices.append(i+2)
        skip = True
    else: 
        skip = False   
segments = np.split(polygon, splitIndices) 
for segment in segments:
    x = [arr[0] for arr in segment.tolist()]
    y = [arr[1] for arr in segment.tolist()]
    plt.plot(x, y, color='red', linewidth=0.1, alpha=1)

#plot Boundaries
boundaries=[[-180,-90],[-180, 90],[180,90],[180,-90],[-180,-90] ]
x,y = zip(*boundaries)
plt.plot(x, y, color='black', linewidth=0.1,  alpha=1)





plt.show()