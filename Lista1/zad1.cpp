#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"
#include <iostream>
#include <vector>

class GameOfLife : public olc::PixelGameEngine
{
public:
	bool runSimulation = false;
	int stepNumber = 0;
	double totalTime = 0;
	std::vector<std::vector<bool>> grid;
	std::vector<std::pair<int, int>> changes;
	std::vector<std::pair<int, int>> adjPositions =
		{{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

public:
	GameOfLife()
	{
		sAppName = "Game of Life";
	}

	bool OnUserCreate() override
	{
		setup();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (stepNumber == 1000)
			std::cout << totalTime << "\n";

		// Events
		if (GetKey(olc::Key::SPACE).bReleased)
			runSimulation = !runSimulation;
		if (GetKey(olc::Key::SHIFT).bReleased)
			nextStep();
		if (GetMouse(0).bHeld)
			grid[GetMouseX()][GetMouseY()] = true;

		// Game
		if (runSimulation)
			nextStep();

		// Drawing
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				if (grid[x][y])
					Draw(x, y, olc::Pixel(255, 255, 255));
				else
					Draw(x, y, olc::Pixel(0, 0, 0));

		// Time
		totalTime += fElapsedTime;

		return true;
	}

	void setup()
	{
		grid.resize(ScreenWidth(), std::vector<bool>(ScreenHeight(), false));
	}

	void nextStep()
	{
		changes.clear();
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
			{
				// Count neighbors
				int neighbors = 0;
				for (auto pos : adjPositions)
				{
					if (x + pos.first < 0 || x + pos.first > grid.size() - 1 || y + pos.second < 0 || y + pos.second > grid[x].size() - 1)
						continue;
					neighbors += grid.at(x + pos.first).at(y + pos.second);
				}

				// Execute rules
				bool alive = grid[x][y];
				if (alive && (neighbors < 2 || neighbors > 3))
					changes.push_back({x, y});
				else if (!alive && neighbors == 3)
					changes.push_back({x, y});
			}

		for (auto pos : changes)
			grid[pos.first][pos.second] = !grid[pos.first][pos.second];
		
		stepNumber++;
	}
};

int main()
{
	GameOfLife game;
	if (game.Construct(100, 100, 5, 5))
		game.Start();

	return 0;
}
