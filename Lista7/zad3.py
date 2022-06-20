import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
import numpy as np

dt = 0.01

sigma = 10
r = 28
b = 8/3

x = [1]
y = [1]
z = [1]

for i in range(10000):
    x.append(x[-1] + (sigma * (y[-1] - x[-1])) * dt)
    y.append(y[-1] + (-x[-2] * z[-1] + r * x[-2] - y[-1]) * dt)
    z.append(z[-1] + (x[-2] * y[-2] - b * z[-1]) * dt)

ax = plt.axes(projection='3d')
ax.plot3D(x, y, z)
plt.show()
