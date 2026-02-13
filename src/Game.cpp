#include "Game.h"
#include "TextureManager.h"
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include "EntityManager.h"
#include "TextManager.h"
#include "ParticleSystem.h"
#include "UIManager.h"
#include "LayoutManager.h"

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
        // Load the file
        LayoutManager::Get().Load("assets/data/layout.json");
        LayoutManager::Get().SetActiveScene("gameplay");
        if (renderer)
        {
            tileset = TextureManager::LoadTexture("assets/kenney_micro-roguelike/Tilemap/colored_tilemap_packed.png", renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created!" << std::endl;
        }

        isRunning = true;
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            std::cout << "SDL_image Error: " << IMG_GetError() << std::endl;
            return;
        }
        if (!TextManager::Initialize())
        {
            std::cout << "Failed to init TextManager!" << std::endl;
            return;
        }
        gameFont = TextManager::LoadFont("assets/fonts/kenney_kenney-fonts/Fonts/Kenney Pixel.ttf", 24);
        UIManager::Get().Init(renderer, gameFont);
        UIManager::Get().AddMessage("Welcome to the Dungeon!", {255, 255, 0, 255}); // Yellow welcome message
        ParticleSystem::Get().Init(1000);
        ParticleSystem::Get().SetFont(gameFont);
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
                if (y == 0 || y == mapHeight - 1 || x == 0 || x == mapWidth - 1)
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
                    if (x < mapWidth - 1 && walls[y * mapWidth + (x + 1)] == 1)
                        mask += 4; // East
                    if (y < mapHeight - 1 && walls[(y + 1) * mapWidth + x] == 1)
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
        // ... inside Game::Init ...

        // 1. Load the Database
        MonsterDatabase::Get().LoadDatabase("assets/data/monsters.json");

        // 2. Spawn 50 Monsters using Real Data
        for (int i = 0; i < 50; i++)
        {
            int rX = (rand() % mapWidth) * 32;
            int rY = (rand() % mapHeight) * 32;

            if (!level->IsWall(rX / 32, rY / 32))
            {

                // Get Random Monster Data
                std::string randomID = MonsterDatabase::Get().GetRandomID();
                const MonsterStats &stats = MonsterDatabase::Get().GetStats(randomID);

                // Create Enemy (You need to update Enemy constructor to take 'stats')
                // For now, if Enemy(x, y, name, sprite) is your constructor:
                Entity *mob = new Enemy(rX, rY, stats);
                EntityManager::Get().AddEntity(mob);
            }
        }

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
        // --- STATE MACHINE ---
        switch (gameState)
        {

        // --- PHASE 1: PLAYER ---
        case GameState::PLAYERTURN:
            if (player->Update(deltaTime, inputManager, level))
            {
                // Player moved! Pass baton to Enemies.
                gameState = GameState::ENEMYTURN;
            }
            break;

        // --- PHASE 2: ENEMIES ---
        case GameState::ENEMYTURN:
            EntityManager::Get().UpdateAll(deltaTime, inputManager, level);
            gameState = GameState::ENVIRONMENT;
            break;

        // --- PHASE 3: ENVIRONMENT ---
        case GameState::ENVIRONMENT:
            // Example:
            // level->UpdateTraps();
            // level->SpreadFire();

            // For now, we just pass through since we have no fire yet.
            gameState = GameState::RESOLVE;
            break;

        // --- PHASE 4: RESOLVE / STATUS EFFECTS ---
        case GameState::RESOLVE:
            EntityManager::Get().Cleanup();
            gameState = GameState::PLAYERTURN;
            break;
        }
        ParticleSystem::Get().Update(deltaTime);

        // --- CAMERA LOGIC ---

        // 1. Get the Game View dimensions from Layout
        UIPanel gameView = LayoutManager::Get().GetPanel("game_view");

        // Default to full screen if not found (safety)
        int viewW = (gameView.id != "") ? gameView.rect.w : 800;
        int viewH = (gameView.id != "") ? gameView.rect.h : 600;

        // 2. Center the camera on the player (Using VIEW sizes)
        camera.x = player->GetPos().x - (viewW / 2);
        camera.y = player->GetPos().y - (viewH / 2);

        // Update camera rect size for rendering culling
        camera.w = viewW;
        camera.h = viewH;

        // --- CAMERA CLAMPING ---

        // Min Bound
        if (camera.x < 0)
            camera.x = 0;
        if (camera.y < 0)
            camera.y = 0;

        // Max Bound (Use VIEW sizes to stop earlier!)
        if (camera.x > (mapWidth * 32) - viewW)
            camera.x = (mapWidth * 32) - viewW;

        if (camera.y > (mapHeight * 32) - viewH)
            camera.y = (mapHeight * 32) - viewH;
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // if (player)
    // {
    //     UIManager::Get().Render(player->GetHP(), player->GetMaxHP());
    // }

    // 1. Get current layout
    const auto &panels = LayoutManager::Get().GetActivePanels();

    // 2. Identify the Game View (if it exists)
    UIPanel gameView = LayoutManager::Get().GetPanel("game_view");

    // --- PASS 1: GAME WORLD ---
    if (gameView.id != "")
    {
        SDL_RenderSetViewport(renderer, &gameView.rect);

        // Draw World
        if (level)
        {
            level->Render(renderer, tileset, camera);
        }
        if (player)
        {
            player->Render(renderer, tileset, camera);
        }
        EntityManager::Get().RenderAll(renderer, tileset, camera);
        ParticleSystem::Get().Render(renderer, tileset, camera);
    }

    // --- PASS 2: UI OVERLAYS ---
    SDL_RenderSetViewport(renderer, NULL); // Reset to full screen

    for (const auto &panel : panels)
    {
        // Skip drawing the "hole" for the game view (optional, purely aesthetic)
        if (panel.id == "game_view")
            continue;
        // A. Background (Fill)
        SDL_SetRenderDrawColor(renderer, panel.bgColor.r, panel.bgColor.g, panel.bgColor.b, panel.bgColor.a);
        SDL_RenderFillRect(renderer, &panel.rect);

        // B. Border (9-Slice OR Simple Line)
        if (!panel.borderImage.empty())
        {
            SDL_Texture *borderTex = TextureManager::LoadTexture(panel.borderImage.c_str(), renderer);

            // 2. Draw 9-Slice Overlay
            if (borderTex)
            {
                TextureManager::SetColor(borderTex, panel.borderColor.r, panel.borderColor.g, panel.borderColor.b);
                TextureManager::SetAlpha(borderTex, panel.borderColor.a);
                TextureManager::Draw9Slice(renderer, borderTex, panel.rect, panel.borderSlice);
                SDL_DestroyTexture(borderTex);
            }
        }
        else
        {
            // Fallback: Simple Line Border
            SDL_SetRenderDrawColor(renderer, panel.borderColor.r, panel.borderColor.g, panel.borderColor.b, panel.borderColor.a);
            SDL_RenderDrawRect(renderer, &panel.rect);
        }

        // C. Content
        UIManager::Get().RenderPanel(renderer, panel, player);
    }
    SDL_RenderPresent(renderer);
}

void Game::Clean()
{
    if (gameFont)
    {
        TTF_CloseFont(gameFont);
        gameFont = nullptr;
    }
    TextManager::Clean();
    if (level)
    {
        delete level;
        level = nullptr;
    }
    EntityManager::Get().ClearAll();
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
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned." << std::endl;
}