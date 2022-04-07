import matplotlib.pyplot as plt

with open("data/zad1.txt") as reader:
    n = []
    nsqrt = []
    radius = []
    for line in reader.readlines():
        data = [float(s) for s in line.split()]
        n.append(data[0])
        nsqrt.append(data[1])
        radius.append(data[2])
    plt.loglog(n, radius)
    plt.loglog(n, nsqrt, label="N^(1/2)")
    
plt.tight_layout()
plt.legend()
plt.xlabel("N")
plt.ylabel("Radius")
plt.show()