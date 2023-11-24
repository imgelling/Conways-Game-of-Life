//#define GAME_USE_DEDICATED_GPU
#define GAME_SUPPORT_OPENGL

#include "Game.h"

constexpr uint32_t MAX_UPDATES = 0;
constexpr uint32_t MIN_UPDATES = 10;
constexpr uint32_t MAX_FRAMES = 0;
constexpr uint32_t MIN_FRAMES = 10;

#define DEAD_COLOR game::Colors::White
#define ALIVE_COLOR game::Colors::Black


class Game : public game::Engine
{

public:
	game::PixelMode pixelMode;
	game::SpriteBatch spriteBatch;
	game::SpriteFont spriteFont;
	game::Pointi worldSize;
	bool* newWorld;
	bool* oldWorld;
	bool running;
	float_t tickSpeed;

	Game() : game::Engine()
	{
		newWorld = nullptr;
		oldWorld = nullptr;
		running = false;
		tickSpeed = 100.0f;
	}

	void HandleWindowSizeChange() override
	{
		if (geIsMinimized)
		{
			geSetGameLocks(MIN_FRAMES, MIN_UPDATES);
		}
		else
		{
			geSetGameLocks(MAX_FRAMES, MAX_UPDATES);
		}
	}

	void Initialize()
	{
		game::Attributes attributes;

		// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		//_CrtSetBreakAlloc(613);
#endif

		attributes.WindowTitle = "Conway's Game of Life";
		attributes.GameVersion = "1.00";
		attributes.FrameLock = MAX_FRAMES;
		attributes.UpdateLock = MAX_UPDATES;
		attributes.VsyncOn = false;
		attributes.DebugMode = false;
		//attributes.MultiSamples = 8; // Needed for GL, check that
		attributes.RenderingAPI = game::RenderAPI::OpenGL;

		geSetAttributes(attributes);

		worldSize.width = 50;
		worldSize.height = 50;
	}

	void LoadContent()
	{
		// Setup renderer(s)
		Setup();

		newWorld = new bool[(size_t)worldSize.width * (size_t)worldSize.height];
		oldWorld = new bool[(size_t)worldSize.width * (size_t)worldSize.height];
		ClearWorld();

		// Setup pixel mode
		if (!pixelMode.Initialize(worldSize))
		{
			geLogLastError();
		}

		// Setup sprite batch
		if (!spriteBatch.Initialize())
		{
			geLogLastError();
		}

		// Load font for output to screen
		if (!spriteFont.Load("Content/new.fnt", "Content/new.png"))
		{
			geLogLastError();
		}

		geMouse.UseMouseAcceleration(false);

		Tick();
	}

	void Shutdown()
	{
		delete newWorld;
		delete oldWorld;
	}

	bool CheckCell(const uint32_t x, const uint32_t y) const
	{
		if (x < 0) return false;
		if (y < 0) return false;
		if (x > (uint32_t)worldSize.width - 1) return false;
		if (y > (uint32_t)worldSize.height - 1) return false;
		return oldWorld[y * worldSize.width + x];
	}

	uint32_t CheckLiveNeighbors(const uint32_t x, const uint32_t y) const
	{
		uint32_t aliveNeighbors = 0;
		// Upper left
		if (CheckCell(x - 1, y - 1)) aliveNeighbors++;
		// Up
		if (CheckCell(x, y - 1)) aliveNeighbors++;
		// Upper right
		if (CheckCell(x + 1, y - 1)) aliveNeighbors++;
		// Right
		if (CheckCell(x + 1, y)) aliveNeighbors++;
		// Down right
		if (CheckCell(x + 1, y + 1)) aliveNeighbors++;
		// Down
		if (CheckCell(x, y + 1)) aliveNeighbors++;
		// Down left
		if (CheckCell(x - 1, y + 1)) aliveNeighbors++;
		// Left
		if (CheckCell(x - 1, y)) aliveNeighbors++;

		return aliveNeighbors;
	}

	void ClearWorld()
	{
		for (uint32_t y = 0; y < (uint32_t)worldSize.height; y++)
		{
			for (uint32_t x = 0; x < (uint32_t)worldSize.width; x++)
			{
				newWorld[y * worldSize.width + x] = false;
				oldWorld[y * worldSize.width + x] = false;
			}
		}
	}

	void Tick()
	{
		uint32_t liveNeighbors = 0;
		uint32_t currentPosition = 0;
		for (uint32_t y = 0; y < (uint32_t)worldSize.height; y++)
		{
			for (uint32_t x = 0; x < (uint32_t)worldSize.width; x++)
			{
				currentPosition = y * worldSize.width + x;
				newWorld[currentPosition] = false;
				liveNeighbors = CheckLiveNeighbors(x, y);
				if (oldWorld[currentPosition]) // Alive
				{
					if (liveNeighbors < 2) // Under Population
					{
						newWorld[currentPosition] = false;
						pixelMode.PixelClip(x, y, DEAD_COLOR);
						continue;
					}
					if (liveNeighbors <= 3) // Lives on
					{
						newWorld[currentPosition] = true;
						pixelMode.PixelClip(x, y, ALIVE_COLOR);
					}
					else // OverPopulation
					{
						newWorld[currentPosition] = false;
						pixelMode.PixelClip(x, y, DEAD_COLOR);
						continue;
					}
				}
				else // Dead
				{
					if (liveNeighbors == 3) // Birth
					{
						newWorld[currentPosition] = true;
						pixelMode.PixelClip(x, y, ALIVE_COLOR);
						continue;
					}
					pixelMode.PixelClip(x, y, DEAD_COLOR);
				}
			}
		}

		// Save the new world for next tick
		memcpy(oldWorld, newWorld, (size_t)worldSize.width * (size_t)worldSize.height * sizeof(bool));
	}

	void Update(const float_t msElapsed)
	{
		// Handle Input
		if (geKeyboard.WasKeyReleased(geK_F11))
		{
			geToggleFullscreen();
		}
		if (geKeyboard.WasKeyReleased(geK_ESCAPE))
		{
			geStopEngine();
		}
		if (geKeyboard.WasKeyReleased(geK_SPACE))
		{
			running = !running;
		}
		if (geKeyboard.WasKeyReleased(geK_COMMA))
		{
			tickSpeed -= 10.0f;
			if (tickSpeed < 0) tickSpeed = 0;
		}	
		if (geKeyboard.WasKeyReleased(geK_PERIOD))
		{
			tickSpeed += 10.0f;
		}
	}

	void Render(const float_t msElapsed)
	{
		static float_t time = 0.0f;
		game::Pointi scaledMousePos = pixelMode.GetScaledMousePosition();

		// Clears and starts new scene
		geClear(GAME_FRAME_BUFFER_BIT | GAME_DEPTH_STENCIL_BUFFER_BIT, game::Colors::DarkGray);

		if (running)
		{
			time += msElapsed;
			if (time >= tickSpeed)
			{
				Tick();
				time -= tickSpeed;
			}
		}
		
		if (geMouse.IsButtonHeld(geMOUSE_LEFT))
		{
			oldWorld[scaledMousePos.y * worldSize.width + scaledMousePos.x] = true;
			pixelMode.PixelClip(scaledMousePos.x, scaledMousePos.y, ALIVE_COLOR);
		}

		if (geMouse.WasButtonReleased(geMOUSE_RIGHT))
		{
			oldWorld[scaledMousePos.y * worldSize.width + scaledMousePos.x] = false;
			pixelMode.PixelClip(scaledMousePos.x, scaledMousePos.y, DEAD_COLOR);
		}

		pixelMode.Render();

		spriteBatch.Begin();
		if (running)
		{
			spriteBatch.DrawString(spriteFont, "Running : True", 0, 0, game::Colors::White, 2.0f);
		}
		else
		{
			spriteBatch.DrawString(spriteFont, "Running : False", 0, 0, game::Colors::White, 2.0f);
		}
		spriteBatch.DrawString(spriteFont, "Tick Time : " + std::to_string((uint32_t)tickSpeed) + "ms", 0, 40, game::Colors::White, 2.0f);
		spriteBatch.DrawString(spriteFont, "FPS : " + std::to_string(geGetFramesPerSecond()), 0, 80, game::Colors::White, 2.0f);
		spriteBatch.DrawString(spriteFont, "UPS : " + std::to_string(geGetUpdatesPerSecond()), 0, 120, game::Colors::White, 2.0f);
		spriteBatch.End();
	}

	// Sets up API stuff
	void Setup() const
	{
#if defined (GAME_OPENGL)
		if (geIsUsing(GAME_OPENGL))
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glEnable(GL_CULL_FACE);
		}
#endif
	}
};

int main()
{
	game::Logger logger("Log.html");
	Game engine;
	engine.geSetLogger(&logger);

	// Create the needed bits for the engine
	if (!engine.geCreate())
	{
		engine.geLogLastError();
		return EXIT_FAILURE;
	}

	// Start the engine
	engine.geStartEngine();

	return EXIT_SUCCESS;
}