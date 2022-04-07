#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"
#include <iostream>
#include <vector>
#include <random>

class Snowflakes : public olc::PixelGameEngine
{
public:
    bool runSimulation = false;
    int width, height;
    int neighborsNeeded = 2;

    std::vector<std::vector<bool>> grid;
    std::mt19937 mt;

public:
    Snowflakes()
    {
        sAppName = "Snowflakes";
    }

    bool OnUserCreate() override
    {
        mt.seed(time(NULL));
        width = ScreenWidth();
        height = ScreenHeight();
        grid.resize(width, std::vector<bool>(height, false));
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Events
        if (GetKey(olc::Key::SPACE).bReleased)
            runSimulation = !runSimulation;
        if (GetKey(olc::Key::SHIFT).bReleased)
            nextStep();
        if (GetMouse(0).bHeld)
            grid[GetMouseX()][GetMouseY()] = true;
        if (GetMouse(1).bHeld)
            grid[GetMouseX()][GetMouseY()] = false;

        // Game
        if (runSimulation)
            nextStep();

        // Drawing
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                if (grid[x][y])
                    Draw(x, y, olc::CYAN);

        return true;
    }

    void nextStep()
    {
        std::vector<std::pair<int, int>> changes;
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
            {
                auto neighbors = getNeighbors(x, y);
                if (!grid[x][y] && neighbors.size() == neighborsNeeded)
                    changes.push_back({x, y});
            }

        for (auto pos : changes)
            grid[pos.first][pos.second] = true;
    }

    std::vector<std::pair<int, int>> getNeighbors(int x, int y)
    {
        std::vector<std::pair<int, int>> neighbors;
        std::vector<std::pair<int, int>> adjPositions{
            {x - 1, y}, {x + 1, y}, {x - (y % 2), y - 1}, {x + !(y % 2), y - 1}, {x - (y % 2), y + 1}, {x + !(y % 2), y + 1}};

        for (auto pos : adjPositions)
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
                continue;
            else if (grid[pos.first][pos.second])
                neighbors.push_back(pos);

        return neighbors;
    }
};

int main()
{
    Snowflakes game;
    if (game.Construct(1001, 1001, 1, 1))
        game.Start();

    return 0;
}
