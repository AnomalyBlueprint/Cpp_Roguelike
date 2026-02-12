#pragma once
#include <SDL.h>
#include <string>
#include "Math/Vector2.h" // Ensure you have this or include Vector2Int and cast

enum class ParticleType
{
    SPRITE, // Uses a texture ID (Blood, Spark)
    TEXT    // Uses a string (Damage Numbers)
};

struct Particle
{
    // --- STATE ---
    bool active = false; // Is this particle in use?
    ParticleType type;

    // --- PHYSICS ---
    float x, y;
    float velX, velY;
    float accX, accY; // Gravity?

    // --- APPEARANCE ---
    int textureID;    // For Sprites
    std::string text; // For Text
    SDL_Color color;

    // --- TWEENING ---
    float lifeTime; // Remaining life
    float maxLife;  // Total life (for % calculation)
    float scale;    // Current size

    // --- SETTINGS ---
    bool fadeAlpha; // Should it fade out?
};