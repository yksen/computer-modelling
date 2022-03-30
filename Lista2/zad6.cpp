#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

const int maxDimension = 2;
bool enableDiagonalMovement = false;

std::mt19937 mt(time(NULL));
std::uniform_int_distribution<int> dirDist(0, 1);
std::uniform_int_distribution<int> dimDist(1, maxDimension);

void randomStep(std::vector<int> *pos)
{
    std::vector<int> dimensions;
    for (int i = 0; i < maxDimension; ++i)
        dimensions.push_back(i);
    std::shuffle(dimensions.begin(), dimensions.end(), mt);

    int dimCount = enableDiagonalMovement ? dimDist(mt) : maxDimension - 1;

    for (int i = 0; i < dimCount; ++i)
        pos->at(dimensions[i]) += dirDist(mt) * 2 - 1;
}

int main()
{
    const int maxSimulationNumber = 1e4;
    const int maxStepNumber = 1e3;

    for (auto state : std::vector<bool>{false, true})
    {
        enableDiagonalMovement = state;
        double squaresSum = 0;
        for (int i = 0; i < maxSimulationNumber; ++i)
        {
            std::vector<int> position(maxDimension, 0);
            for (int j = 0; j < maxStepNumber; ++j)
                randomStep(&position);
            for (auto coord : position)
                squaresSum += std::pow(coord, 2);
        }
        double msd = squaresSum / maxSimulationNumber;
        std::cout << "Results:\n";
        std::cout << "> Diagonal movement enabled = " << enableDiagonalMovement << "\n";
        std::cout << "> Mean square displacement = " << msd << "\n";
        std::cout << "> t = " << maxStepNumber << "\n";
        std::cout << "> Diffusion = " << msd / (2 * maxDimension * maxStepNumber) << "\n";
    }
}