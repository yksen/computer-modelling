import matplotlib.pyplot as plt

with open("data/zad3/data.txt") as reader:
    distance = []
    for line in reader.readlines():
        distance.append(int(line))
    test = plt.hist(distance, max(distance) - min(distance), align="left", density=True)
    plt.plot(test[1][:-1:2], test[0][::2])
    plt.xlabel("Odległość od punktu startowego")
    plt.ylabel("Prawdopodobieństwo")
    plt.tight_layout()
    plt.show()