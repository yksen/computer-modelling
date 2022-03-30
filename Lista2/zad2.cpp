#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"
#include <iostream>
#include <vector>
#include <random>
#include <time.h>

class RandomWalk : public olc::PixelGameEngine
{
public:
    bool runSimulation = false;

    int width, height;
    int dimensions = 2;
    int numberOfPeople = 14;

    std::vector<std::vector<std::pair<int, int>>> routes;
    std::vector<olc::Pixel> colors = {olc::RED, olc::YELLOW, olc::GREEN, olc::CYAN, olc::BLUE, olc::MAGENTA, olc::WHITE};
    std::mt19937 mt;

public:
    RandomWalk()
    {
        sAppName = "Random walk";
    }

    bool OnUserCreate() override
    {
        width = ScreenWidth();
        height = ScreenHeight();
        if (dimensions == 1)
        {
            numberOfPeople = height;
            routes.resize(numberOfPeople, std::vector<std::pair<int, int>>(1, {width / 2, 0}));
            for (int i = 0; i < routes.size(); ++i)
                routes[i].back().second += i;
        }
        else if (dimensions == 2)
            routes.resize(numberOfPeople, std::vector<std::pair<int, int>>(1, {width / 2, height / 2}));
        mt.seed(time(NULL));
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Events
        if (GetKey(olc::Key::SPACE).bReleased)
            runSimulation = !runSimulation;
        if (GetKey(olc::Key::SHIFT).bReleased)
            nextStep(dimensions);

        // Game
        if (runSimulation)
            nextStep(dimensions);

        // Drawing
        Clear(olc::Pixel(0, 0, 0));
        for (int i = 0; i < routes[0].size(); ++i)
            for (int j = 0; j < routes.size(); ++j)
            {
                auto color = colors[j % colors.size()];
                color.a = i * (255.0 / routes[j].size());
                Draw(routes[j][i].first, routes[j][i].second, color);
            }

        return true;
    }

    void nextStep(int d)
    {
        std::uniform_int_distribution<int> direction(0, 2 * d - 1);

        for (auto &route : routes)
        {
            auto pos = route.back();
            switch (direction(mt))
            {
            case 0:
                pos.first--;
                break;
            case 1:
                pos.first++;
                break;
            case 2:
                pos.second--;
                break;
            case 3:
                pos.second++;
                break;
            }
            route.push_back(pos);
        }
    }
};

int main()
{
    RandomWalk window;
    if (window.Construct(960, 540, 2, 2, true))
        window.Start();

    return 0;
}
