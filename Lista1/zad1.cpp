#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>

class GameOfLife : public olc::PixelGameEngine
{
public:
	enum Pattern {
		glider
	};

	int width, height;
	bool runSimulation = false;

	int stepNumber = 0;
	int population = 0;

	double totalTime = 0;

	std::vector<std::vector<bool>> grid;
	std::vector<std::pair<int, int>> changes;
	std::vector<std::pair<int, int>> adjPositions =
		{{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

	std::ofstream file;

public:
	GameOfLife()
	{
		sAppName = "Game of Life";
	}

	bool OnUserCreate() override
	{
		width = ScreenWidth();
		height = ScreenHeight();
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
		if (GetKey(olc::Key::G).bReleased)
			placePattern(GetMouseX(), GetMouseY());
		if (GetMouse(0).bHeld)
			grid[GetMouseX()][GetMouseY()] = true;
		if (GetMouse(1).bHeld)
			grid[GetMouseX()][GetMouseY()] = false;

		// Game
		if (runSimulation)
			nextStep();

		// Drawing
		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
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
		grid.resize(width, std::vector<bool>(height, false));
	}

	void nextStep()
	{
		population = 0;
		changes.clear();
		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
			{
				// Count population
				population += grid[x][y];

				// Count neighbors
				int neighbors = 0;
				for (auto pos : adjPositions)
					neighbors += grid[(x + pos.first + width) % width][(y + pos.second + height) % height];

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

	void placePattern(int x, int y)
	{
		grid[x][(y - 1 + height) % height] = true;
		grid[(x - 1 + width) % width][y] = true;
		grid[(x - 1 + width) % width][(y + 1 + height) % height] = true;
		grid[x][(y + 1 + height) % height] = true;
		grid[(x + 1 + width) % width][(y + 1 + height) % height] = true;
	}
};

int main()
{
	GameOfLife game;
	if (game.Construct(100, 100, 10, 10))
		game.Start();

	return 0;
}
