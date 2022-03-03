#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"
#include <iostream>
#include <vector>

class GameOfLife : public olc::PixelGameEngine
{
public:
	bool runSimulation = false;
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
					try
					{
						neighbors += grid.at(x + pos.first).at(y + pos.second);
					}
					catch (...)
					{
					}
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
	}
};

int main()
{
	GameOfLife game;
	if (game.Construct(256, 256, 4, 4, false, true))
		game.Start();

	return 0;
}
