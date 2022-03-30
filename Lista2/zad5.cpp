#include <vector>
#include <random>
#include <algorithm>
#include <fstream>

std::mt19937 mt(time(NULL));
std::uniform_int_distribution<int> dirDist(0, 1);

void randomStep(std::vector<int> *pos)
{
    std::vector<int> dimensions = {0, 1, 2};
    std::shuffle(dimensions.begin(), dimensions.end(), mt);
    
    std::uniform_int_distribution<int> dimDist(1, pos->size());
    int dimCount = dimDist(mt);

    for (int i = 0; i < dimCount; ++i)
        pos->at(dimensions[i]) += dirDist(mt) * 2 - 1;
}

int main()
{
    const int dimension = 3;
    const int maxStepNumber = 1e3;
    std::vector<int> position(dimension, 0);

    std::ofstream file;
    file.open("data/zad5/data.txt");
    for (int i = 0; i < maxStepNumber; ++i)
    {
        for (auto coord : position)
            file << coord << " ";
        file << "\n";
        randomStep(&position);
    }
    file.close();
}