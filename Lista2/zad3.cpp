#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <fstream>

std::mt19937 mt;
std::vector<int> routes;

void nextStep()
{
    std::uniform_int_distribution<int> direction(0, 1);

    for (auto &route : routes)
    {
        auto pos = route;
        switch (direction(mt))
        {
        case 0:
            pos--;
            break;
        case 1:
            pos++;
            break;
        }
        route = pos;
    }
}

int main()
{
    mt.seed(time(NULL));
    const int N = 1000;
    int numberOfPeople = 1e6;
    routes.resize(numberOfPeople, 0);

    for (int i = 0; i < N; ++i)
        nextStep();

    std::ofstream file;
    file.open("data/zad3/data.txt");
    for (auto route : routes)
        file << route << "\n";
    file.close();
}