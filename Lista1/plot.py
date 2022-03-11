from cProfile import label
import os
import matplotlib.pyplot as plt

path = "data/zad2/"
files = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f))]

for file in files:
    with open(os.path.join(path, file)) as reader:
        x = []
        y = []
        for line in reader.readlines():
            data = [int(s) for s in line.split() if s.isdigit()]
            x.append(data[0])
            y.append(data[1] / 10000)
        plt.plot(x, y, label=file[5:9], linewidth=0.75)

plt.xlabel("Czas")
plt.ylabel("Gęstość")
plt.legend()
plt.show()