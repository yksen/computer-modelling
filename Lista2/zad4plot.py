from cProfile import label
import matplotlib.pyplot as plt

with open("data/zad4/data.txt") as reader:
    maxStepNumber = []
    firstDim = []
    secondDim = []
    thirdDim = []
    for line in reader.readlines():
        data = [float(s) for s in line.split()]
        maxStepNumber.append(data[0])
        firstDim.append(data[1])
        secondDim.append(data[2])
        thirdDim.append(data[3])
    plt.scatter(maxStepNumber, firstDim, label="1D")
    plt.scatter(maxStepNumber, secondDim, label="2D")
    plt.scatter(maxStepNumber, thirdDim, label="3D")
    plt.xlabel("Maksymalna ilość kroków")
    plt.ylabel("Prawdopodobieństwo powrotu na start")
    plt.legend()
    plt.tight_layout()
    plt.show()