#define OLC_PGE_APPLICATION
#include "../../include/olcPixelGameEngine.h"
#include <bits/stdc++.h>

class Sand : public olc::PixelGameEngine
{
public:
    int width, height;

    int sandRadius = 50;
    int groundRadius = 2;

    int c = 1;

    float p = 0.9;
    std::mt19937 mt;
    std::uniform_real_distribution<float> dist{0, 1};

    std::vector<std::vector<int>> sand1;
    std::vector<std::vector<int>> sand2;
    std::vector<std::vector<int>> ground;

public:
    Sand()
    {
        sAppName = "Sand";
    }

    bool OnUserCreate() override
    {
        width = ScreenWidth();
        height = ScreenHeight();

        mt.seed(time(NULL));

        sand1.resize(height, std::vector<int>(width, 0));
        sand2.resize(height, std::vector<int>(width, 0));
        ground.resize(height, std::vector<int>(width, 0));

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        if (GetMouse(0).bHeld)
            for (int x = GetMouseX() - sandRadius; x < GetMouseX() + sandRadius; x++)
                for (int y = GetMouseY() - sandRadius; y < GetMouseY() + sandRadius; y++)
                    if (x >= 0 && x < width && y >= 0 && y < height)
                        if (GetKey(olc::Key::SHIFT).bHeld)
                            sand1[x][y] = 0;
                        else
                            sand1[x][y] = 1;
        if (GetMouse(1).bHeld)
            for (int x = GetMouseX() - groundRadius; x < GetMouseX() + groundRadius; x++)
                for (int y = GetMouseY() - groundRadius; y < GetMouseY() + groundRadius; y++)
                    if (x >= 0 && x < width && y >= 0 && y < height)
                        if (GetKey(olc::Key::SHIFT).bHeld)
                            ground[x][y] = 0;
                        else
                            ground[x][y] = 1;

        for (int x = c; x < width; x += 2)
            for (int y = c; y < height; y += 2)
            {
                if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
                    continue;

                int Sul = sand1[x][y];
                int Sur = sand1[x + 1][y];
                int Sll = sand1[x][y + 1];
                int Slr = sand1[x + 1][y + 1];

                int Gul = ground[x][y];
                int Gur = ground[x + 1][y];
                int Gll = ground[x][y + 1];
                int Glr = ground[x + 1][y + 1];

                if (Sul == 1 && Sur == 1 && Sll == 0 && Slr == 0 && Gll == 0 && Glr == 0)
                {
                    if (dist(mt) < p)
                    {
                        sand2[x][y] = 0;
                        sand2[x + 1][y] = 0;
                        sand2[x][y + 1] = 1;
                        sand2[x + 1][y + 1] = 1;
                    }
                }
                else
                {
                    sand2[x][y] = Gul * Sul + (1 - Gul) * Sul * Sll * (Slr + (1 - Slr) * Sur);
                    sand2[x + 1][y] = Gur * Sur + (1 - Gur) * Sur * Slr * (Sll + (1 - Sll) * Sul);
                    sand2[x][y + 1] = Sll + (1 - Sll) * (Sul * (1 - Gul) + (1 - Sul) * Sur * (1 - Gur) * Slr);
                    sand2[x + 1][y + 1] = Slr + (1 - Slr) * (Sur * (1 - Gur) + (1 - Sur) * Sul * (1 - Gul) * Sll);
                }
            }

        Clear(olc::BLACK);
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                if (sand1[x][y] == 1)
                    Draw(x, y, olc::YELLOW);
                else if (ground[x][y] == 1)
                    Draw(x, y, olc::WHITE);

        sand1 = sand2;
        c = 1 - c;

        return true;
    }
};

int main()
{
    Sand app;
    if (app.Construct(512, 512, 2, 2))
        app.Start();

    return 0;
}
