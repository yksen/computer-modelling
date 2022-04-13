import matplotlib.pyplot as plt
import numpy as np

with open("data/zad2.txt") as reader:
    n = []
    radius = []
    for line in reader.readlines():
        data = [float(s) for s in line.split()]
        n.append(data[0])
        radius.append(data[1])
    linreg = np.polyfit(n, radius, 1)
    print(linreg)
    plt.loglog(n, radius, label=str(linreg[1]) + "x + " + str(linreg[0]))

plt.tight_layout()
plt.legend()
plt.xlabel("N")
plt.ylabel("Radius")
plt.show()