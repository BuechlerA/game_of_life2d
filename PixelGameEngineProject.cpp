#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <math.h>

// Override base class with your custom functionality
class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		// Name you application
		sAppName = "Conways's Game of Life in C++";
	}
private:

	int* m_output;
	int* m_state;

	const int simLimit = { 50 };
	int tickRate = { 50 };

	float penSize = { 0.0f };

public:
	bool isRunning;

public:
	bool OnUserCreate() override
	{
		m_output = new int[ScreenWidth() * ScreenHeight()];
		m_state = new int[ScreenWidth() * ScreenHeight()];
		memset(m_output, 0, ScreenWidth() * ScreenHeight() * sizeof(int));
		memset(m_state, 0, ScreenWidth() * ScreenHeight() * sizeof(int));

		isRunning = false;

		////Set all cells to a random state on start up
		//for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
		//{
		//	m_state[i] = rand() % 2;
		//}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		auto cell = [&](int x, int y) {
			return m_output[y * ScreenWidth() + x];
		};

		if (GetKey(olc::SPACE).bHeld)
		{
			isRunning = true;
		}
		else
		{
			isRunning = false;
		}

		if (GetKey(olc::UP).bPressed) tickRate += 10;
		if (GetKey(olc::DOWN).bPressed) tickRate -= 10;

		if (GetMouseWheel() >= 0) penSize += 1.0f;
		if (GetMouseWheel() <= 0) penSize -= 1.0f;
		if (penSize < 0) penSize = 0;

		//Pause state sequence
		if (!isRunning)
		{
			//Clear(olc::BLACK);

			if (GetMouse(0).bHeld)
			{
				m_output[GetMouseY() * ScreenWidth() + GetMouseX()] = 1;
				m_state[GetMouseY() * ScreenWidth() + GetMouseX()] = 1;
			}

			if (GetMouse(1).bHeld)
			{
				m_output[GetMouseY() * ScreenWidth() + GetMouseX()] = 0;
				m_state[GetMouseY() * ScreenWidth() + GetMouseX()] = 0;
			}

			for (int width = 0; width < ScreenWidth(); width++)
			{
				for (int height = 0; height < ScreenHeight(); height++)
				{
					if (cell(width, height) == 1)
					{						
						Draw(width, height, olc::BLUE);
					}
					else
					{
						Draw(width, height, olc::BLACK);
					}
				}
			}

			Draw(GetMouseX(), GetMouseY(), olc::GREEN);

			//resizeable brush function to be implemented below

			////midpoint rect is (p1 + q2) / 2 + (p2 + q2 / 2)
			//olc::vf2d midpoint = { (GetMouseX() + (GetMouseX() + penSize)) / 2, (GetMouseY() + (GetMouseY() + penSize)) / 2 };
			////distance is sqrt ( (x2 - x1) ^ 2 + (y2 - y1) ^ 2)
			//float distance = sqrtf(pow(GetMouseX() + midpoint.x - GetMouseX(), 2) + pow(GetMouseY() + midpoint.y - GetMouseY(), 2));
			//DrawRect(olc::vi2d(midpoint.x - GetMouseX(), midpoint.y - GetMouseY()), olc::vi2d(penSize, penSize), olc::GREEN);

			////DEBUG STUFF
			//Draw(midpoint, olc::RED);
			//DrawStringDecal(olc::vf2d(5.0f, 12.0f), "midpoint: " + std::to_string(midpoint.x) + "," + std::to_string(midpoint.y), olc::GREEN, olc::vf2d(0.3f, 0.3f));
			//DrawStringDecal(olc::vf2d(5.0f, 15.0f), "mouse: " + std::to_string(GetMouseX()) + "," + std::to_string(GetMouseY()), olc::GREEN, olc::vf2d(0.3f, 0.3f));
			//DrawStringDecal(olc::vf2d(5.0f, 18.0f), "endpoint: " + std::to_string(GetMouseX() + penSize) + "," + std::to_string(GetMouseY() + penSize), olc::GREEN, olc::vf2d(0.3f, 0.3f));
			//DrawStringDecal(olc::vf2d(5.0f, 21.0f), "distance: " + std::to_string(distance), olc::GREEN, olc::vf2d(0.3f, 0.3f));
		}

		//Running state sequence
		if (isRunning)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(tickRate));
			
			for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
			{
				m_output[i] = m_state[i];
			}

			for (int x = 0; x < ScreenWidth(); x++)
			{
				for (int y = 0; y < ScreenHeight(); y++)
				{
					int nNeighbours =
						cell(x - 1, y - 1) + cell(x + 0, y - 1) + cell(x + 1, y - 1) +
						cell(x - 1, y + 0) + 0 + cell(x + 1, y + 0) +
						cell(x - 1, y + 1) + cell(x + 0, y + 1) + cell(x + 1, y + 1);

					if (cell(x, y) == 1)
						m_state[y * ScreenWidth() + x] = nNeighbours == 2 || nNeighbours == 3;
					else
						m_state[y * ScreenWidth() + x] = nNeighbours == 3;

					if (cell(x, y) == 1)
						Draw(x, y, olc::WHITE);
					else
						Draw(x, y, olc::BLACK);
				}
			}
		}


		//DrawString(5, 5, std::to_string(isRunning), olc::GREEN);
		//DrawString(5, 15, std::to_string(tickRate), olc::GREEN);
		DrawStringDecal(olc::vf2d( 5.0f, 5.0f ), "state: " + GetRunningState(), olc::GREEN, olc::vf2d( 0.3f, 0.3f));
		DrawStringDecal(olc::vf2d(5.0f, 8.0f), "tick rate: " + std::to_string(tickRate), olc::GREEN, olc::vf2d(0.3f, 0.3f));

		return true;
	}

	std::string GetRunningState()
	{
		std::string stateMsg;

		if (isRunning)
		{
			stateMsg = std::string("running");
		}
		else
		{
			stateMsg = std::string("paused");
		}

		return stateMsg;
	}
};

int main()
{
	Game demo;
	if (demo.Construct(100, 100, 8, 8))
		demo.Start();
	return 0;
}