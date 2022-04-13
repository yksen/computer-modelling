#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <fstream>
#include <cmath>

struct Cell
{
    bool alive;
    bool inCluster;
    bool onBorder;
    Cell(bool x, bool y, bool z) : alive(x), inCluster(y), onBorder(z) {}
    Cell() : alive(false), inCluster(false), onBorder(false) {}
};

class DLA : public olc::PixelGameEngine
{
public:
    bool runSimulation = true;
    int width, height;
    int furthestCoordinate;
    int n = 0;
    int prevCount = 0;

    std::vector<std::vector<Cell>> grid;
    std::vector<std::pair<int, int>> adjPositions =
        {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};

    std::mt19937 mt;

    std::ofstream file;

public:
    DLA()
    {
        sAppName = "DLA";
    }

    bool OnUserCreate() override
    {
        mt.seed(time(NULL));
        width = ScreenWidth();
        height = ScreenHeight();
        furthestCoordinate = 0;
        grid.resize(width, std::vector<Cell>(height));
        grid[width / 2][height / 2] = Cell(true, true, true);
        spawnWanderers();
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Events
        if (GetKey(olc::Key::SPACE).bReleased)
            runSimulation = !runSimulation;
        if (GetKey(olc::Key::SHIFT).bReleased)
            nextStep();
        if (GetKey(olc::Key::S).bReleased)
            spawnWanderers();
        if (GetMouse(0).bHeld)
            grid[GetMouseX()][GetMouseY()] = Cell(true, false, false);
        if (GetMouse(1).bHeld)
            grid[GetMouseX()][GetMouseY()] = Cell(true, true, false);

        // Game
        if (runSimulation)
            nextStep();

        // Drawing
        Clear(olc::BLACK);
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                if (grid[x][y].alive)
                    if (grid[x][y].inCluster)
                        if (grid[x][y].onBorder)
                            Draw(x, y, olc::DARK_GREEN);
                        else
                            Draw(x, y, olc::GREEN);
                    else
                        Draw(x, y, olc::YELLOW);
        drawRadius();

        return true;
    }

    void spawnWanderers()
    {
        double radius = furthestCoordinate + 10;
        std::uniform_int_distribution<int> dist(0, 9);
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                if (std::pow(x - width / 2, 2) + std::pow(y - height / 2, 2) > radius * radius && !dist(mt))
                    grid[x][y] = Cell(true, false, false);
    }

    void nextStep()
    {
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                if (grid[x][y].alive && !grid[x][y].inCluster)
                {
                    auto neighbors = getAdjacent(x, y);
                    std::vector<std::vector<std::pair<int, int>>::iterator> toErase;
                    for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
                        if (grid[it->first][it->second].inCluster)
                            grid[x][y] = Cell(true, true, true);
                        else if (grid[it->first][it->second].alive)
                            toErase.push_back(it);

                    for (auto it = toErase.begin(); it != toErase.end(); ++it)
                        neighbors.erase(*it);

                    if (neighbors.size() > 0 && !grid[x][y].inCluster)
                    {
                        std::uniform_int_distribution<int> dist(0, neighbors.size() - 1);
                        int index = dist(mt);
                        grid[neighbors[index].first][neighbors[index].second] = Cell(true, false, false);
                        grid[x][y] = Cell(false, false, false);
                    }
                }

        ++n;
    }

    std::vector<std::pair<int, int>> getAdjacent(int x, int y)
    {
        std::vector<std::pair<int, int>> adjCells;
        for (auto adjPos : adjPositions)
            if (!(x == 0 || x == width - 1 || y == 0 || y == height - 1))
                adjCells.push_back(std::pair<int, int>{x + adjPos.first, y + adjPos.second});
        return adjCells;
    }

    void drawRadius()
    {
        std::pair<int, int> centerofMass{0, 0};
        int aliveCount = 0;
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                if (grid[x][y].alive && grid[x][y].inCluster)
                {
                    int coordinate = std::max(std::abs(x - width / 2), std::abs(y - height / 2));
                    if (coordinate > furthestCoordinate)
                        furthestCoordinate = coordinate;
                    centerofMass.first += x;
                    centerofMass.second += y;
                    ++aliveCount;
                }
        centerofMass.first /= aliveCount;
        centerofMass.second /= aliveCount;

        double radius = 0;
        int borderCount = 0;
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                if (grid[x][y].alive && grid[x][y].inCluster && grid[x][y].onBorder)
                {
                    radius += std::sqrt(std::pow(x - centerofMass.first, 2) + std::pow(y - centerofMass.second, 2));
                    ++borderCount;
                }
        radius /= borderCount;

        if (prevCount != aliveCount)
        {
            file.open("data/zad2.txt", std::ofstream::app);
            file << aliveCount << " " << radius << "\n";
            file.close();
        }
        prevCount = aliveCount;

        DrawCircle(centerofMass.first, centerofMass.second, radius);
    }
};

int main()
{
    DLA game;
    if (game.Construct(501, 501, 2, 2))
        game.Start();

    return 0;
}
