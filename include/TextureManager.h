#pragma once
#include <SDL.h>

class TextureManager
{
public:
    // Loads a BMP file and converts it to a hardware texture
    static SDL_Texture *LoadTexture(const char *fileName, SDL_Renderer *renderer);

    // Draws a portion of the texture (srcRect) to the screen (destRect)
    static void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_Renderer *renderer);
    static void SetColor(SDL_Texture *tex, Uint8 r, Uint8 g, Uint8 b);
    static void SetAlpha(SDL_Texture *tex, Uint8 alpha);
    static void SetBlendMode(SDL_Texture *tex, SDL_BlendMode mode);
};