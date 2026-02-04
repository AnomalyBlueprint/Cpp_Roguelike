#include "Game.h"
#include "TextureManager.h" 
#include <cstdlib> 
#include <ctime>                                

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), player(nullptr), inputManager(nullptr), lastTime(0), lag(0.0) {}

Game::~Game() {}

void Game::Init(const char *title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        std::cout << "Subsystems Initialized!..." << std::endl;

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (renderer)
        {
            tileset = TextureManager::LoadTexture("assets/tileset_colored.bmp", renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created!" << std::endl;
        }

        isRunning = true;

        // Create a 20x15 map (fits in 800x600 with 32px tiles roughly)
        level = new TileMap(25, 19, 32);

        // In Game::Init...

        std::vector<int> ground(25 * 19, 0); // Fill with ID 0 (Grass)
        for (int i = 0; i < 25 * 19; i++)
        {
            int rng = std::rand() % 100; // Pick a number from 0 to 99

            if (rng < 80)
            {
                ground[i] = 0; // 80% Chance: Standard Grass (ID 0)
            }
            else if (rng < 95)
            {
                ground[i] = 1; // 15% Chance: Variant Grass (ID 1)
            }
            else
            {
                ground[i] = 2; // 5% Chance: Flowers (ID 2)
            }
        }
        std::vector<int> walls(25 * 19, 0);  // Fill with 0 (Empty)
        // 1. First pass: Place generic "Wall Markers" (ID 1) everywhere you want a wall.
        // We aren't picking the sprite yet, just saying "There is a wall here."
        for (int y = 0; y < 19; y++)
        {
            for (int x = 0; x < 25; x++)
            {
                int index = y * 25 + x;
                // Simple Logic: Borders are walls
                if (y == 0 || y == 18 || x == 0 || x == 24)
                {
                    walls[index] = 1; // Temporary "Wall" flag
                }
                // Random Pillar
                else if (x == 10 && y == 10)
                {
                    walls[index] = 1;
                }
            }
        }

        // 2. Second pass: BITMASKING + DIRECTIONAL CHECK
        std::vector<int> finalWalls = walls;

        for (int y = 0; y < 19; y++)
        {
            for (int x = 0; x < 25; x++)
            {
                int index = y * 25 + x;

                if (walls[index] == 1)
                {
                    int mask = 0;

                    // Safe Boundary Checks (Treat Out-of-Bounds as 'Empty' for now)
                    if (y > 0 && walls[(y - 1) * 25 + x] == 1)
                        mask += 1; // North
                    if (x > 0 && walls[y * 25 + (x - 1)] == 1)
                        mask += 2; // West
                    if (x < 24 && walls[y * 25 + (x + 1)] == 1)
                        mask += 4; // East
                    if (y < 18 && walls[(y + 1) * 25 + x] == 1)
                        mask += 8; // South

                    int tileID = AssetRegistry::Get().GetAutoTile(mask);

                    // --- THE FIX: DISAMBIGUATE SYMMETRY ---

                    // If it's a Vertical Line (Mask 9), check "West" to see if it's Floor
                    if (mask == 9)
                    {
                        // If the tile to the LEFT is Floor (0), this must be a RIGHT WALL
                        if (x > 0 && walls[y * 25 + (x - 1)] == 0)
                        {
                            tileID = 12; // Force Right Wall ID
                        }
                        else
                        {
                            tileID = 11; // Force Left Wall ID
                        }
                    }

                    // If it's a Horizontal Line (Mask 6), check "North" to see if it's Floor
                    if (mask == 6)
                    {
                        // If the tile Above is Floor, this is a Bottom Wall
                        if (y > 0 && walls[(y - 1) * 25 + x] == 0)
                        {
                            tileID = 13; // Force Bottom Wall ID
                        }
                        else
                        {
                            tileID = 10; // Force Top Wall ID
                        }
                    }

                    finalWalls[index] = tileID;
                }
            }
        }

        // Load the final polished map
        level->LoadLayers(ground, finalWalls);

        // Init Subsystems
        inputManager = new InputManager();
        player = new Player(32*3, 32*3);

        lastTime = SDL_GetTicks64();
    }
    else
    {
        isRunning = false;
    }
}

// THIS IS THE MAIN ACCUMULATOR LOOP
void Game::Run()
{
    while (isRunning)
    {
        Uint64 current = SDL_GetTicks64();
        Uint64 elapsed = current - lastTime;
        lastTime = current;

        // Convert to milliseconds (double)
        double elapsedMS = (double)elapsed;
        lag += elapsedMS;

        // 1. INPUT (Poll events)
        inputManager->Prepare();
        HandleEvents();

        // 2. FIXED UPDATE (Catch up physics)
        while (lag >= MS_PER_UPDATE)
        {
            FixedUpdate();
            lag -= MS_PER_UPDATE;
        }

        // 3. UPDATE (Variable Delta Time for Visuals)
        // Convert elapsed MS to Seconds for standard Update
        Update((float)(elapsedMS / 1000.0));

        // 4. RENDER
        Render();
    }
}

void Game::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        // Feed the Input Manager
        if (inputManager)
        {
            inputManager->HandleEvent(event);
        }
    }
}

void Game::FixedUpdate()
{
    // This runs exactly 60 times per second (or whatever MS_PER_UPDATE is).
    // Put physics, reliable timers, or turn-tick counters here.
    // For now, we leave it empty.
}

void Game::Update(float deltaTime)
{
    // This runs as fast as the screen allows (VSync).
    // Use this for Animations, UI, and interpolation.

    if (player && level)
    {
        player->Update(deltaTime, inputManager, level);
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black BG
    SDL_RenderClear(renderer);

    if (level)
    {
        level->Render(renderer, tileset);
    }
    if (player)
    {
        player->Render(renderer, tileset);
    }

    SDL_RenderPresent(renderer);
}

void Game::Clean()
{
    if (level)
    {
        delete level;
        level = nullptr;
    }
    if (player)
    {
        delete player;
        player = nullptr;
    }
    if (inputManager)
    {
        delete inputManager;
        inputManager = nullptr;
    }
    if (tileset)
    {
        SDL_DestroyTexture(tileset);
        tileset = nullptr;
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned." << std::endl;
}