#pragma once
#include <vector>
#include <SDL_ttf.h>
#include "Particle.h"
#include "TextureManager.h"
#include "AssetConfigs.h"

class ParticleSystem
{
public:
    static ParticleSystem &Get()
    {
        static ParticleSystem instance;
        return instance;
    }

    // Initialize the pool
    void Init(int poolSize = 1000)
    {
        pool.resize(poolSize);
    }

    // --- SPAWNERS ---

    // Spawn Sprites (e.g., Blood Splatter)
    void EmitSprite(int textureID, int x, int y, int count, SDL_Color color);

    // Spawn Text (e.g., Damage Numbers)
    void EmitText(const std::string &text, int x, int y, SDL_Color color);

    // --- LOOP ---
    void Update(float deltaTime);
    void Render(SDL_Renderer *renderer, SDL_Texture *spritesheet, SDL_Rect camera);

    // Set the font for damage numbers
    void SetFont(TTF_Font *font) { damageFont = font; }

private:
    std::vector<Particle> pool;
    TTF_Font *damageFont = nullptr;

    ParticleSystem() {} // Private Constructor

    // Helper: Find a dead particle to reuse
    Particle *GetFreeParticle()
    {
        for (auto &p : pool)
        {
            if (!p.active)
                return &p;
        }
        return nullptr; // Pool full!
    }
};