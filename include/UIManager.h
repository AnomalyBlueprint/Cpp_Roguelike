#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include "TextManager.h"
#include "LayoutManager.h"
#include "Player.h"

class UIManager
{
public:
    static UIManager &Get()
    {
        static UIManager instance;
        return instance;
    }

    void Init(SDL_Renderer *renderer, TTF_Font *font)
    {
        this->renderer = renderer;
        this->font = font;
    }

    // --- 1. THE LOG SYSTEM ---
    void AddMessage(const std::string &msg, SDL_Color color = {255, 255, 255, 255})
    {
        logs.push_back({msg, color});
        if (logs.size() > 5)
        { // Keep only last 5 messages
            logs.erase(logs.begin());
        }
    }

    // --- 2. THE RENDERER ---
    void Render(int playerHP, int maxHP)
    {
        // A. Draw Player HP Bar (Top Left)
        DrawBar(10, 10, 200, 20, playerHP, maxHP, {200, 0, 0, 255}); // Red Bar

        // Label
        TextManager::Draw(renderer, font, "HP: " + std::to_string(playerHP), 15, 8, {255, 255, 255, 255});

        // B. Draw Message Log (Bottom Left)
        int y = 500; // Start near bottom
        for (const auto &log : logs)
        {
            TextManager::Draw(renderer, font, "> " + log.msg, 10, y, log.color);
            y += 20; // Move down line by line
        }
    }

    void RenderPanel(SDL_Renderer *renderer, const UIPanel &panel, Player *player)
    {

        // --- 1. STATS PANEL ---
        if (panel.id == "stats_panel" && player)
        {
            int x = panel.rect.x + 10;
            int y = panel.rect.y + 10;
            int lineHeight = 25;

            // Draw Header
            TextManager::Draw(renderer, font, "--- STATS ---", x, y, {255, 255, 0, 255});
            y += lineHeight * 1.5;

            // HP Bar Background
            SDL_Rect barBg = {x, y, 150, 20};
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_RenderFillRect(renderer, &barBg);

            // HP Bar Foreground
            float hpPercent = (float)player->GetHP() / (float)player->GetMaxHP();
            SDL_Rect barFg = {x, y, (int)(150 * hpPercent), 20};
            SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); // Red
            SDL_RenderFillRect(renderer, &barFg);

            // HP Text
            std::string hpText = std::to_string(player->GetHP()) + " / " + std::to_string(player->GetMaxHP());
            TextManager::Draw(renderer, font, hpText, x + 40, y + 2, {255, 255, 255, 255});
            y += lineHeight * 2;

            // Other Stats (Placeholder)
            TextManager::Draw(renderer, font, "STR: " + std::to_string(player->GetDamage()), x, y, {200, 200, 200, 255});
            y += lineHeight;
            TextManager::Draw(renderer, font, "DEF: " + std::to_string(player->GetArmor()), x, y, {200, 200, 200, 255});
            y += lineHeight;
        }

        // --- 2. MESSAGE LOG ---
        else if (panel.id == "message_log")
        {
            int x = panel.rect.x + 10;
            int y = panel.rect.y + 10;

            for (const auto &log : logs)
            {
                // Simple Clipping: Stop if we go past the panel height
                if (y > panel.rect.y + panel.rect.h - 20)
                    break;

                TextManager::Draw(renderer, font, "> " + log.msg, x, y, log.color);
                y += 20;
            }
        }
    }

private:
    struct LogEntry
    {
        std::string msg;
        SDL_Color color;
    };

    SDL_Renderer *renderer;
    TTF_Font *font;
    std::vector<LogEntry> logs;

    // Helper to draw a filled bar
    void DrawBar(int x, int y, int w, int h, int current, int max, SDL_Color color)
    {
        if (max <= 0)
            max = 1;
        float percent = (float)current / (float)max;
        if (percent < 0)
            percent = 0;

        // Background (Dark Gray)
        SDL_Rect bg = {x, y, w, h};
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &bg);

        // Foreground (Health Color)
        SDL_Rect fg = {x, y, (int)(w * percent), h};
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &fg);

        // Border (White)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &bg);
    }

    UIManager() {}
};