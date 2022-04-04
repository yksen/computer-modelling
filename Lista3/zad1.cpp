#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <random>
#include <fstream>

struct Cell
{
    bool alive = false;
    bool onBorder = false;
};

class Eden : public olc::PixelGameEngine
{
public:
    bool runSimulation = false;
    int width, height;
    int n = 0;
    char variant = 'B';

    std::vector<std::vector<Cell>> grid;
    std::vector<std::pair<int, int>> adjPositions =
        {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};

    std::mt19937 mt;

    std::ofstream file;

public:
    Eden()
    {
        sAppName = "Eden";
    }

    bool OnUserCreate() override
    {
        mt.seed(time(NULL));
        width = ScreenWidth();
        height = ScreenHeight();
        grid.resize(width, std::vector<Cell>(height));
        grid[width / 2][height / 2].alive = true;
        grid[width / 2][height / 2].onBorder = true;
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
			grid[GetMouseX()][GetMouseY()].alive = true;
		if (GetMouse(1).bHeld)
			grid[GetMouseX()][GetMouseY()].alive = false;

        // Game
        if (runSimulation)
            nextStep();

        // Drawing
        Clear(olc::BLACK);
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                if (grid[x][y].alive)
                    if (grid[x][y].onBorder)
                        Draw(x, y, olc::DARK_GREEN);
                    else
                        Draw(x, y, olc::GREEN);
        drawRadius();

        return true;
    }

    void nextStep()
    {
        auto candidates = getCandidates();

        std::uniform_int_distribution<int> dist(0, candidates.size() - 1);
        auto randCell = candidates[dist(mt)];
        grid[randCell.first][randCell.second].alive = true;

        ++n;
    }

    std::vector<std::pair<int, int>> getCandidates()
    {
        std::vector<std::pair<int, int>> candidates;

        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
            {
                if (!grid[x][y].alive)
                    continue;
                auto neighbors = getAdjacent(x, y);
                if (neighbors.size() > 0)
                {
                    candidates.insert(candidates.end(), neighbors.begin(), neighbors.end());
                    grid[x][y].onBorder = true;
                }
                else
                    grid[x][y].onBorder = false;
            }

        return candidates;
    }

    std::vector<std::pair<int, int>> getAdjacent(int x, int y)
    {
        std::vector<std::pair<int, int>> adjCells;
        for (auto adjPos : adjPositions)
        {
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
                continue;
            if (!grid[x + adjPos.first][y + adjPos.second].alive)
                adjCells.push_back(std::pair<int, int>{x + adjPos.first, y + adjPos.second});
        }
        return adjCells;
    }

    void drawRadius()
    {
        std::pair<int, int> centerofMass{0, 0};
        int aliveCount = 0;
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                if (grid[x][y].alive)
                {
                    centerofMass.first += x;
                    centerofMass.second += y;
                    ++aliveCount;
                }
        centerofMass.first /= aliveCount;
        centerofMass.second /= aliveCount;

        int radius = 0;
        int borderCount = 0;
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                if (grid[x][y].alive && grid[x][y].onBorder)
                {
                    radius += std::sqrt(std::pow(x - centerofMass.first, 2) + std::pow(y - centerofMass.second, 2));
                    ++borderCount;
                }
        radius /= borderCount;

        file.open("data/zad1.txt", std::ofstream::app);
        file << n << " " << std::pow(n, 0.5) << " " << radius << "\n";
        file.close();

        DrawCircle(centerofMass.first, centerofMass.second, radius);
    }
};

int main()
{
    Eden game;
    if (game.Construct(501, 501, 2, 2))
        game.Start();

    return 0;
}
