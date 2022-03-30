import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

with open("data/zad5/data.txt") as reader:
    x = []
    y = []
    z = []
    for line in reader.readlines():
        data = [float(s) for s in line.split()]
        x.append(data[0])
        y.append(data[1])
        z.append(data[2])

    fig = plt.figure(figsize=(4, 4))
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlim(min(x + y + z), max(x + y + z))
    ax.set_ylim(min(x + y + z), max(x + y + z))
    ax.set_zlim(min(x + y + z), max(x + y + z))
    ax.plot(x, y, z)
    ax.plot(0, 0, 0, "o")
    plt.tight_layout()
    plt.show()
