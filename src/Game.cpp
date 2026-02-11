#include "Game.h"
#include "TextureManager.h"
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), player(nullptr), inputManager(nullptr), lastTime(0), lag(0.0) {}

Game::~Game() {}

void Game::Init(const char *title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        std::cout << "Subsystems Initialized!..." << std::endl;

        mapWidth = 60;
        mapHeight = 40;
        level = new TileMap(mapWidth, mapHeight, 32);
        camera = {0, 0, 800, 600};

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (renderer)
        {
            tileset = TextureManager::LoadTexture("assets/tileset_colored.bmp", renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created!" << std::endl;
        }

        isRunning = true;

        // Seed the random number generator so every run is unique
        std::srand(std::time(nullptr));

        // Create the map container
        level = new TileMap(mapWidth, mapHeight, 32);

        // --- 1. GENERATE GROUND LAYER (Weighted Random) ---
        // Instead of filling with 0, we roll the dice for every tile.
        std::vector<int> ground(mapWidth * mapHeight, 0);

        for (int i = 0; i < mapWidth * mapHeight; i++)
        {
            int rng = std::rand() % 100; // 0 to 99

            if (rng < 80)
            {
                ground[i] = 0; // 80% Chance: Standard Grass
            }
            else if (rng < 95)
            {
                ground[i] = 1; // 15% Chance: Variant Grass (Darker/Tuft)
            }
            else
            {
                ground[i] = 2; // 5% Chance: Flowers
            }
        }

        // --- 2. GENERATE WALL LAYER (Bitmasking) ---
        std::vector<int> walls(mapWidth * mapHeight, 0);

        // Pass A: Place generic "Wall Markers" (ID 1)
        for (int y = 0; y < mapHeight; y++)
        {
            for (int x = 0; x < mapWidth; x++)
            {
                int index = y * mapWidth + x;

                // Borders
                if (y == 0 || y == mapHeight-1 || x == 0 || x == mapWidth-1)
                {
                    walls[index] = 1;
                }
                // Random Pillar
                else if (x == 10 && y == 10)
                {
                    walls[index] = 1;
                }
            }
        }

        // Pass B: Apply Bitmasking Logic
        std::vector<int> finalWalls = walls;

        for (int y = 0; y < mapHeight; y++)
        {
            for (int x = 0; x < mapWidth; x++)
            {
                int index = y * mapWidth + x;

                if (walls[index] == 1)
                {
                    int mask = 0;

                    if (y > 0 && walls[(y - 1) * mapWidth + x] == 1)
                        mask += 1; // North
                    if (x > 0 && walls[y * mapWidth + (x - 1)] == 1)
                        mask += 2; // West
                    if (x < mapWidth-1 && walls[y * mapWidth + (x + 1)] == 1)
                        mask += 4; // East
                    if (y < mapHeight-1 && walls[(y + 1) * mapWidth + x] == 1)
                        mask += 8; // South

                    int tileID = AssetRegistry::Get().GetAutoTile(mask);

                    // Disambiguate Vertical Lines (Left vs Right Wall)
                    if (mask == 9)
                    {
                        if (x > 0 && walls[y * mapWidth + (x - 1)] == 0)
                            tileID = 12; // Right Wall
                        else
                            tileID = 11; // Left Wall
                    }

                    // Disambiguate Horizontal Lines (Top vs Bottom Wall)
                    if (mask == 6)
                    {
                        if (y > 0 && walls[(y - 1) * mapWidth + x] == 0)
                            tileID = 13; // Bottom Wall
                        else
                            tileID = 10; // Top Wall
                    }

                    finalWalls[index] = tileID;
                }
            }
        }

        // Load the finalized layers into the map
        level->LoadLayers(ground, finalWalls);

        // Init Subsystems
        inputManager = new InputManager();
        player = new Player(32 * 3, 32 * 3);

        lastTime = SDL_GetTicks64();
    }
    else
    {
        isRunning = false;
    }
}

void Game::Run()
{
    while (isRunning)
    {
        Uint64 current = SDL_GetTicks64();
        Uint64 elapsed = current - lastTime;
        lastTime = current;

        double elapsedMS = (double)elapsed;
        lag += elapsedMS;

        inputManager->Prepare();
        HandleEvents();

        while (lag >= MS_PER_UPDATE)
        {
            FixedUpdate();
            lag -= MS_PER_UPDATE;
        }

        Update((float)(elapsedMS / 1000.0));
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
        if (inputManager)
        {
            inputManager->HandleEvent(event);
        }
    }
}

void Game::FixedUpdate()
{
    // Physics or Turn Logic could go here
}

void Game::Update(float deltaTime)
{
    if (player && level)
    {
        player->Update(deltaTime, inputManager, level);

        // --- CAMERA LOGIC ---

        // 1. Center the camera on the player
        // Camera X = Player X - Half Screen Width
        camera.x = player->GetPos().x - (800 / 2);
        camera.y = player->GetPos().y - (600 / 2);

        // --- CAMERA CLAMPING ---

        // 1. Min Bound (Left & Top)
        // Prevents seeing the black void on the Left/Top sides
        if (camera.x < 0)
            camera.x = 0;
        if (camera.y < 0)
            camera.y = 0;

        // 2. Max Bound (Right & Bottom)
        // Prevents seeing the black void on the Right/Bottom sides
        // Logic: The camera stops when its Right Edge hits the Map's Right Edge.
        if (camera.x > (mapWidth * 32) - camera.w)
            camera.x = (mapWidth * 32) - camera.w;

        if (camera.y > (mapHeight * 32) - camera.h)
            camera.y = (mapHeight * 32) - camera.h;
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (level)
    {
        level->Render(renderer, tileset, camera);
    }
    if (player)
    {
        player->Render(renderer, tileset, camera);
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