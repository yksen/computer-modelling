#include <iostream>
#include <vector>

int main()
{
    float S, I, R;
    float nS, nI, nR;
    float beta, gamma;
    float dt = 0;

    S = 1000;
    I = 4;
    R = 0;
    float N = S + I + R;

    beta = 0.4;
    gamma = 0.04;

    for (int i = 0; i < 1000; ++i)
    {
        nS = S - (beta * I * S / N) * dt;
        nI = I + (beta * I * S / N - gamma * I) * dt;
        nR = R + (gamma * I) * dt;
        S = nS;
        I = nI;
        R = nR;

        std::cout << S << "\t" << I << "\t" << R << "\t" << dt << "\n";
        dt += 0.01;
    }

    return 0;
}