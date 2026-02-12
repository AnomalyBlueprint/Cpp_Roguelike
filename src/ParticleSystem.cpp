#include "ParticleSystem.h"
#include <cmath>
#include <cstdlib>

void ParticleSystem::EmitSprite(int textureID, int x, int y, int count, SDL_Color color)
{
    for (int i = 0; i < count; i++)
    {
        Particle *p = GetFreeParticle();
        if (!p)
            return;

        p->active = true;
        p->type = ParticleType::SPRITE;
        p->x = (float)x;
        p->y = (float)y;
        p->textureID = textureID;
        p->color = color;

        // Random Explosion Velocity (360 degrees)
        float angle = (rand() % 360) * (3.14159f / 180.0f);
        float speed = (rand() % 100) + 50.0f; // Speed 50-150
        p->velX = cos(angle) * speed;
        p->velY = sin(angle) * speed;

        p->accX = 0;
        p->accY = 0;

        p->lifeTime = 0.5f + ((rand() % 50) / 100.0f); // 0.5s - 1.0s
        p->maxLife = p->lifeTime;
        p->scale = 1.0f;
        p->fadeAlpha = true;
    }
}

void ParticleSystem::EmitText(const std::string &text, int x, int y, SDL_Color color)
{
    Particle *p = GetFreeParticle();
    if (!p)
        return;

    p->active = true;
    p->type = ParticleType::TEXT;
    p->x = (float)x;
    p->y = (float)y;
    p->text = text;
    p->color = color;

    // Float Upwards
    p->velX = (float)((rand() % 40) - 20); // Slight horizontal drift
    p->velY = -50.0f;                      // Upward speed
    p->accX = 0;
    p->accY = 0;

    p->lifeTime = 0.8f;
    p->maxLife = 0.8f;
    p->scale = 0.5f; // Start small
    p->fadeAlpha = true;
}

void ParticleSystem::Update(float dt)
{
    for (auto &p : pool)
    {
        if (!p.active)
            continue;

        p.lifeTime -= dt;
        if (p.lifeTime <= 0)
        {
            p.active = false;
            continue;
        }

        // Physics
        p.velX += p.accX * dt;
        p.velY += p.accY * dt;
        p.x += p.velX * dt;
        p.y += p.velY * dt;

        // --- THE JUICE (TWEENING) ---
        if (p.type == ParticleType::TEXT)
        {
            float t = 1.0f - (p.lifeTime / p.maxLife); // 0.0 -> 1.0

            // "Punch" Effect: Grow fast, then shrink slightly
            if (t < 0.2f)
            {
                p.scale = 0.5f + (t * 5.0f); // 0.5 -> 1.5 quickly
            }
            else
            {
                p.scale = 1.5f - ((t - 0.2f) * 0.5f); // 1.5 -> 1.1 slowly
            }
        }
    }
}

void ParticleSystem::Render(SDL_Renderer *renderer, SDL_Texture *spritesheet, SDL_Rect camera)
{
    for (auto &p : pool)
    {
        if (!p.active)
            continue;

        SDL_Rect dest;
        dest.x = (int)p.x - camera.x;
        dest.y = (int)p.y - camera.y;

        // Calculate Alpha
        int alpha = 255;
        if (p.fadeAlpha)
        {
            float t = p.lifeTime / p.maxLife; // 1.0 -> 0.0
            alpha = (int)(t * 255);
            if (alpha < 0)
                alpha = 0;
        }

        if (p.type == ParticleType::SPRITE)
        {
            const TileData &data = AssetRegistry::Get().GetTile(p.textureID);
            dest.w = 32 * p.scale; // Assuming 32x32 sprites
            dest.h = 32 * p.scale;

            // We need a way to draw with Alpha.
            // Standard TextureManager::Draw might not support AlphaMod yet.
            SDL_SetTextureColorMod(spritesheet, p.color.r, p.color.g, p.color.b);
            SDL_SetTextureAlphaMod(spritesheet, alpha);
            SDL_RenderCopy(renderer, spritesheet, &data.srcRect, &dest);

            // Reset
            SDL_SetTextureColorMod(spritesheet, 255, 255, 255);
            SDL_SetTextureAlphaMod(spritesheet, 255);
        }
        else if (p.type == ParticleType::TEXT && damageFont)
        {
            // Render Text directly
            SDL_Surface *surf = TTF_RenderText_Solid(damageFont, p.text.c_str(), p.color);
            if (surf)
            {
                SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);

                dest.w = surf->w * p.scale;
                dest.h = surf->h * p.scale;

                SDL_SetTextureAlphaMod(tex, alpha);
                SDL_RenderCopy(renderer, tex, NULL, &dest);

                SDL_FreeSurface(surf);
                SDL_DestroyTexture(tex);
            }
        }
    }
}