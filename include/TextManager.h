#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

class TextManager
{
public:
    static bool Initialize()
    {
        if (TTF_Init() == -1)
        {
            std::cout << "[Error] SDL_ttf Init: " << TTF_GetError() << std::endl;
            return false;
        }
        return true;
    }

    static void Clean()
    {
        TTF_Quit();
    }

    // Load a font
    static TTF_Font *LoadFont(const std::string &path, int fontSize)
    {
        TTF_Font *font = TTF_OpenFont(path.c_str(), fontSize);
        if (!font)
        {
            std::cout << "[Error] Failed to load font: " << path << std::endl;
        }
        return font;
    }

    // Draw Text to Screen
    static void Draw(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, int x, int y, SDL_Color color)
    {
        if (!font)
            return;

        // 1. Render Text to Surface
        SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!surface)
            return;

        // 2. Convert Surface to Texture
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        // 3. Draw Texture
        SDL_Rect dest = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &dest);

        // 4. Cleanup (Crucial! Otherwise memory leak)
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
};