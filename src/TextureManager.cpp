#include "TextureManager.h"
#include <iostream>

SDL_Texture *TextureManager::LoadTexture(const char *fileName, SDL_Renderer *renderer)
{
    // 1. Load Image to "Surface" (CPU Memory)
    SDL_Surface *tempSurface = SDL_LoadBMP(fileName);

    if (!tempSurface)
    {
        std::cout << "Failed to load texture: " << fileName << "\n";
        return nullptr;
    }

    // 2. Convert Surface to "Texture" (GPU Memory)
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, tempSurface);

    // 3. Clean up the Surface (we don't need CPU copy anymore)
    SDL_FreeSurface(tempSurface);

    return tex;
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_Renderer *renderer)
{
    // RenderCopy is what draws textures instead of shapes
    SDL_RenderCopy(renderer, tex, &src, &dest);
}
void TextureManager::SetColor(SDL_Texture *tex, Uint8 r, Uint8 g, Uint8 b)
{
    SDL_SetTextureColorMod(tex, r, g, b);
}

void TextureManager::SetAlpha(SDL_Texture *tex, Uint8 alpha)
{
    SDL_SetTextureAlphaMod(tex, alpha);
}

void TextureManager::SetBlendMode(SDL_Texture *tex, SDL_BlendMode mode)
{
    SDL_SetTextureBlendMode(tex, mode);
}