#define OLC_PGE_APPLICATION
#define _USE_MATH_DEFINES
#include "../include/olcPixelGameEngine.h"
#include <bits/stdc++.h>

const int W = 1024;
const int H = 1024;

const size_t N = 10;

class DoublePendulum : public olc::PixelGameEngine
{
public:
    double dt = 0.1;
    double g = 1.0;
    double m1 = 1.0;
    double m2 = 1.0;

    double theta1[N];
    double theta2[N];
    double theta1prim[N];
    double theta2prim[N];
    double theta1bis[N];
    double theta2bis[N];

    double L1 = W / 4, L2 = W / 4;
    double px0 = W / 2, py0 = H / 2;

public:
    DoublePendulum()
    {
        sAppName = "DoublePendulum";
    }

    bool OnUserCreate() override
    {
        for (size_t i = 0; i < N; ++i)
        {
            theta1[i] = M_PI * 0.56;
            theta2[i] = M_PI * 0.56 + 0.00001 * (i / float(N) - 0.5);
            theta1prim[i] = 0;
            theta2prim[i] = 0;
            theta1bis[i] = 0;
            theta2bis[i] = 0;
        }

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        if (GetKey(olc::Key::ESCAPE).bPressed)
            return false;

        Clear(olc::BLACK);

        for (size_t i = 0; i < N; ++i)
        {
            theta1[i] = theta1[i] + theta1prim[i] * dt;
            theta2[i] = theta2[i] + theta2prim[i] * dt;

            theta1bis[i] = -g * (2 * m1 + m2) * sin(theta1[i]) - m2 * g * sin(theta1[i] - 2 * theta2[i]) - 2 * sin(theta1[i] - theta2[i]) * m2 * (theta2prim[i] * theta2prim[i] * L2 + theta1prim[i] * theta1prim[i] * L1 * cos(theta1[i] - theta2[i]));
            theta1bis[i] /= L1 * (2 * m1 + m2 - m2 * cos(2 * theta1[i] - 2 * theta2[i]));

            theta2bis[i] = 2 * sin(theta1[i] - theta2[i]) * (theta1prim[i] * theta1prim[i] * L1 * (m1 + m2) + g * (m1 + m2) * cos(theta1[i]) + theta2prim[i] * theta2prim[i] * L2 * m2 * cos(theta1[i] - theta2[i]));
            theta2bis[i] /= L2 * (2 * m1 + m2 - m2 * cos(2 * theta1[i] - 2 * theta2[i]));

            theta1prim[i] = theta1prim[i] + theta1bis[i] * dt;
            theta2prim[i] = theta2prim[i] + theta2bis[i] * dt;

            double x1 = px0 + L1 * sin(theta1[i]);
            double y1 = py0 + L1 * cos(theta1[i]);
            double x2 = x1 + L2 * sin(theta2[i]);
            double y2 = y1 + L2 * cos(theta2[i]);
            
            DrawLine(px0, py0, x1, y1, olc::GREEN);
            DrawLine(x1, y1, x2, y2, olc::GREEN);
        }

        return true;
    }
};

int main()
{
    DoublePendulum app;
    if (app.Construct(W, H, 1, 1))
        app.Start();

    return 0;
}
