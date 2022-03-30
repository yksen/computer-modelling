import matplotlib.pyplot as plt

with open("data/zad1/data.txt") as reader:
    n = []
    rand = []
    mt = []
    for line in reader.readlines():
        data = [int(s) for s in line.split() if s.isdigit()]
        n.append(data[0])
        rand.append(data[1])
        mt.append(data[2])
    plt.scatter(n, rand, label="rand()")
    plt.scatter(n, mt, label="Mersenne-Twister")

plt.tight_layout()
plt.legend()
plt.xlabel("N")
plt.ylabel("Liczebność kubełka")
plt.show()