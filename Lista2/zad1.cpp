#include <random>
#include <iostream>
#include <vector>
#include <fstream>

int main()
{
    const int N = 10000;
    const int sampleSize = 1e9;
    std::vector<int> randHistogram(N, 0);
    std::vector<int> mtHistogram(N, 0);
    
    // Rand
    srand(time(NULL));
    rand();

    // Mersenne Twister
    std::mt19937 mt(time(NULL));
    std::uniform_real_distribution<double> dist(0, 1.0);

    // Random generation
    for (int i = 0; i < sampleSize; ++i)
    {
        randHistogram[(rand() / float(RAND_MAX)) * N]++;
        mtHistogram[dist(mt) * N]++;
    }

    // Write data to file
    std::ofstream file;
    file.open("data/zad1/data.txt");
    for (int i = 0; i < N; ++i)
        file << i << "\t" << randHistogram[i] << "\t" << mtHistogram[i] << "\n";
    file.close();

    return 0;
}