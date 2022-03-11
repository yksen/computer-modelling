#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"
#include <iostream>
#include <vector>
#include <random>
#include <math.h>

class GameOfLife : public olc::PixelGameEngine
{
public:
	bool runSimulation = true;
	bool drawSimulation = false;
	int width, height;
    int gridSize;

	int stepNumber = 0;
	int simulationNumber = 0;
	int population = 0;
	double populationSum = 0;
	std::vector<double> averagePopulations;
	double totalTime = 0;
	double prevTime = 0;

    const int maxStepNumber = 1000;
    const int maxSimulationNumber = 100;
	const double probability = 0.60;
	
	std::vector<std::vector<bool>> grid;
	std::vector<std::pair<int, int>> changes;
	std::vector<std::pair<int, int>> adjPositions =
		{{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
	
public:
	GameOfLife(int size = 100)
	{
		sAppName = "Game of Life";
		gridSize = size;
	}

	bool OnUserCreate() override
	{
		width = gridSize;
		height = gridSize;
		setup();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{		
		// Events
		if (drawSimulation)
		{
			if (GetKey(olc::Key::SPACE).bReleased)
				runSimulation = !runSimulation;
			if (GetKey(olc::Key::SHIFT).bReleased)
				nextStep();
			if (GetMouse(0).bHeld)
				grid[GetMouseX()][GetMouseY()] = true;
			if (GetMouse(1).bHeld)
				grid[GetMouseX()][GetMouseY()] = false;
		}

		// Simulation
		if (runSimulation)
			nextStep();

		// Drawing
		if (drawSimulation)
			for (int x = 0; x < width; x++)
				for (int y = 0; y < height; y++)
					if (grid[x][y])
						Draw(x, y, olc::Pixel(255, 255, 255));
					else
						Draw(x, y, olc::Pixel(0, 0, 0));

        // Next simulation
        if (stepNumber == maxStepNumber)
		{
			if (simulationNumber < maxSimulationNumber)
			{
                std::cout << "Simulation number: " << simulationNumber << "\tTime elapsed: " << totalTime - prevTime << "\tDensity: " << (populationSum / maxStepNumber) / (width * height) << "\n";
				prevTime = totalTime;
				averagePopulations.push_back(populationSum / maxStepNumber);
				populationSum = 0;
				simulationNumber++;
				stepNumber = 0;
				setup();
			}
			else
            {
                std::cout << "Done in " << totalTime << " seconds.\n";
				
				double sum = 0;
				for (double pop : averagePopulations)
					sum += pop / (width * height);
				double average = sum / maxSimulationNumber;
				double devSum = 0;
				for (double pop : averagePopulations)
					devSum += std::pow(pop / (width * height) - average, 2);
				double sd = std::sqrt(devSum / maxSimulationNumber);
				double standardError = sd / std::sqrt(maxSimulationNumber);

				std::cout << "Results:\n";
				std::cout << "> Sum = " << sum << "\n";
				std::cout << "> Average = " << average << "\n";
				std::cout << "> Sum of deviations = " << devSum << "\n";
				std::cout << "> Standard deviation = " << sd << "\n";
				std::cout << "> Standard error = " << standardError << "\n";

				return false;
            }
		}

		// Time
		totalTime += fElapsedTime;

		return true;
	}

	void setup()
	{
		grid.resize(width, std::vector<bool>(height, false));

		std::mt19937 rng(time(0));
		std::uniform_int_distribution<int> dist(0, 100);
		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
				grid[x][y] = dist(rng) < (probability * 100);
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

		populationSum += population;
		stepNumber++;
	}
};

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		GameOfLife game(std::stoi(argv[1]));
		if (game.Construct(10, 10, 10, 10))
			game.Start();
	}

	return 0;
}
