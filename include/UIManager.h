#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <sys/stat.h> // For mkdir
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

// 1. Create logs directory (Cross-platform safe-ish)
#ifdef _WIN32
        mkdir("logs");
#else
        mkdir("logs", 0777);
#endif

        // 2. Create a unique Session File (e.g., logs/session_2023-10-27_10-00.txt)
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << "logs/session_" << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S") << ".txt";

        logFile.open(oss.str());
        if (logFile.is_open())
        {
            logFile << "=== Game Session Started ===\n";
        }
    }

    void Clean()
    {
        if (logFile.is_open())
        {
            logFile << "=== Session Ended ===\n";
            logFile.close();
        }
    }

    // --- LOG SYSTEM ---
    void AddMessage(const std::string &msg, SDL_Color color = {255, 255, 255, 255})
    {
        // 1. Save to File
        if (logFile.is_open())
        {
            logFile << "> " << msg << "\n";
            logFile.flush();
        }

        // 2. Add to Memory
        logs.push_back({msg, color});

        // 3. Smart Scroll Logic
        if (scrollOffset > 0)
        {
            // If user is looking at history, mark as "Unread"
            hasNewMessages = true;
        }
        // If scrollOffset is 0, we automatically stay at the bottom
    }

    // --- INPUT (For Scrolling) ---
    void HandleInput(SDL_Event &event)
    {
        if (event.type == SDL_MOUSEWHEEL)
        {
            if (event.wheel.y > 0)
            { // Scroll UP (History)
                scrollOffset++;
            }
            else if (event.wheel.y < 0)
            { // Scroll DOWN (Newer)
                if (scrollOffset > 0)
                    scrollOffset--;
            }

            // Clamp (Don't scroll past the oldest message)
            int maxScroll = (int)logs.size() - 5; // Keep at least 5 lines visible
            if (scrollOffset > maxScroll)
                scrollOffset = maxScroll;
            if (scrollOffset < 0)
                scrollOffset = 0;

            // If we are at the bottom, clear the "New" flag
            if (scrollOffset == 0)
                hasNewMessages = false;
        }
    }

    // Call this when clicking the "New V" button
    void ScrollToBottom()
    {
        scrollOffset = 0;
        hasNewMessages = false;
    }

    // --- RENDER ---
    void RenderPanel(SDL_Renderer *renderer, const UIPanel &panel, Player *player)
    {
        if (panel.id == "message_log")
        {
            RenderLog(panel);
        }
        else if (panel.id == "stats_panel" && player)
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
    std::ofstream logFile;

    // Scrolling State
    int scrollOffset = 0;        // 0 = At Bottom. 1 = 1 line up.
    bool hasNewMessages = false; // "New V" flag

    void RenderLog(const UIPanel &panel)
    {
        int x = panel.rect.x + 10;
        int bottomY = panel.rect.y + panel.rect.h - 10;
        int lineHeight = 20;

        // How many lines fit?
        int maxVisible = (panel.rect.h - 20) / lineHeight;

        // Render from Bottom-Up
        int drawn = 0;
        int startIdx = (int)logs.size() - 1 - scrollOffset;

        for (int i = startIdx; i >= 0; i--)
        {
            if (drawn >= maxVisible)
                break;

            int y = bottomY - ((drawn + 1) * lineHeight);
            TextManager::Draw(renderer, font, "> " + logs[i].msg, x, y, logs[i].color);
            drawn++;
        }

        // --- DRAW "NEW V" POPUP ---
        if (hasNewMessages)
        {
            SDL_Rect btn = {
                panel.rect.x + panel.rect.w - 70,
                panel.rect.y + panel.rect.h - 25,
                60, 20};
            SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); // Green
            SDL_RenderFillRect(renderer, &btn);
            TextManager::Draw(renderer, font, "New v", btn.x + 5, btn.y + 2, {0, 0, 0, 255});
        }
    }

    UIManager() {}
    ~UIManager() { Clean(); }
};