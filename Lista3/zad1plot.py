import matplotlib.pyplot as plt

with open("data/zad1.txt") as reader:
    nsqrt = []
    radius = []
    for line in reader.readlines():
        data = [float(s) for s in line.split()]
        nsqrt.append(data[1])
        radius.append(data[2])
    plt.plot(nsqrt, radius)
    
plt.tight_layout()
plt.xlabel("N^(1/2)")
plt.ylabel("Radius")
plt.show()