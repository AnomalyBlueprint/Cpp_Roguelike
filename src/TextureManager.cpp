#include "TextureManager.h"
#include <SDL_image.h>
#include <iostream>

SDL_Texture *TextureManager::LoadTexture(const char *fileName, SDL_Renderer *renderer)
{
    // OLD: SDL_Surface* tempSurface = SDL_LoadBMP(fileName);

    // NEW: Handles PNG, JPG, BMP, etc.
    SDL_Surface *tempSurface = IMG_Load(fileName);

    if (!tempSurface)
    {
        std::cout << "[Error] Failed to load texture: " << fileName << " Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
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

void TextureManager::Draw9Slice(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect dest, const std::vector<int> &slice)
{
    if (!texture)
        return;

    // Safety check: ensure we have 4 slice values (Top, Right, Bottom, Left)
    // If not, just draw normally.
    if (slice.size() < 4)
    {
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        return;
    }

    int texW, texH;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    int top = slice[0];
    int right = slice[1];
    int bottom = slice[2];
    int left = slice[3];

    // Source Patches (The 9 regions of the PNG)
    SDL_Rect src[9];

    // Row 1 (Top)
    src[0] = {0, 0, left, top};                   // Top-Left
    src[1] = {left, 0, texW - left - right, top}; // Top-Center
    src[2] = {texW - right, 0, right, top};       // Top-Right

    // Row 2 (Middle)
    src[3] = {0, top, left, texH - top - bottom};                   // Middle-Left
    src[4] = {left, top, texW - left - right, texH - top - bottom}; // Center
    src[5] = {texW - right, top, right, texH - top - bottom};       // Middle-Right

    // Row 3 (Bottom)
    src[6] = {0, texH - bottom, left, bottom};                   // Bottom-Left
    src[7] = {left, texH - bottom, texW - left - right, bottom}; // Bottom-Center
    src[8] = {texW - right, texH - bottom, right, bottom};       // Bottom-Right

    // Destination Patches (Where they go on screen)
    int dW = dest.w;
    int dH = dest.h;
    int dX = dest.x;
    int dY = dest.y;

    SDL_Rect dst[9];

    // Row 1
    dst[0] = {dX, dY, left, top};
    dst[1] = {dX + left, dY, dW - left - right, top};
    dst[2] = {dX + dW - right, dY, right, top};

    // Row 2
    dst[3] = {dX, dY + top, left, dH - top - bottom};
    dst[4] = {dX + left, dY + top, dW - left - right, dH - top - bottom};
    dst[5] = {dX + dW - right, dY + top, right, dH - top - bottom};

    // Row 3
    dst[6] = {dX, dY + dH - bottom, left, bottom};
    dst[7] = {dX + left, dY + dH - bottom, dW - left - right, bottom};
    dst[8] = {dX + dW - right, dY + dH - bottom, right, bottom};

    // Draw all 9 patches
    for (int i = 0; i < 9; i++)
    {
        // Skip drawing if width or height is 0/negative
        if (src[i].w > 0 && src[i].h > 0 && dst[i].w > 0 && dst[i].h > 0)
        {
            SDL_RenderCopy(renderer, texture, &src[i], &dst[i]);
        }
    }
}