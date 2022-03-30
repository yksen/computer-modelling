#include <iostream>
#include <vector>
#include <random>

std::mt19937 mt(time(NULL));
std::uniform_int_distribution<int> dist(0, 1);

void randomStep(std::vector<int> *pos)
{
    std::uniform_int_distribution<int> dimDist(0, pos->size() - 1);
    int randomDimension = dimDist(mt);
    pos->at(randomDimension) += dist(mt) * 2 - 1;
}

int main()
{
    const int maxSimulationNumber = 1e3;
    std::vector<int> maxStepNumbers;
    for (int i = 10; i <= 1000; i += 10)
        maxStepNumbers.push_back(i);
    const std::vector<int> dimensions = {1, 2, 3};

    for (auto maxStepNumber : maxStepNumbers)
    {
        std::cout << maxStepNumber << " ";
        for (auto dimension : dimensions)
        {
            double probability = 0;
            for (int i = 0; i < maxSimulationNumber; ++i)
            {
                std::vector<int> position(dimension, 0);
                int comebackCount = 0;
                
                for (int j = 0; j < maxStepNumber; ++j)
                {
                    randomStep(&position);
                    if (position == std::vector<int>(dimension, 0))
                        ++comebackCount;
                }
                probability += float(comebackCount) / maxStepNumber;
            }
            probability /= maxSimulationNumber;
            std::cout << probability << " ";
        }
        std::cout << "\n";
    }
}