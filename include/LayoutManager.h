#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

struct UIPanel
{
    std::string id;
    SDL_Rect rect;
    SDL_Color bgColor;
    SDL_Color borderColor;
    std::string layoutType;

    // --- NEW: 9-Slice Data ---
    std::string borderImage;      // Path to PNG (e.g., "assets/ui/border.png")
    std::vector<int> borderSlice; // [Top, Right, Bottom, Left]
};

class LayoutManager
{
public:
    static LayoutManager &Get()
    {
        static LayoutManager instance;
        return instance;
    }

    void Load(const std::string &path)
    {
        std::ifstream f(path);
        if (!f.is_open())
        {
            std::cerr << "[Error] Layout file not found: " << path << std::endl;
            return;
        }

        try
        {
            json data = json::parse(f);
            scenes.clear();

            for (auto &[sceneName, panels] : data["scenes"].items())
            {
                std::vector<UIPanel> panelList;

                for (auto &p : panels)
                {
                    UIPanel panel;
                    panel.id = p["id"];
                    panel.rect = {
                        p["geometry"]["x"], p["geometry"]["y"],
                        p["geometry"]["w"], p["geometry"]["h"]};

                    // Parse Background Color
                    auto bg = p["style"]["bg_color"];
                    panel.bgColor = {(Uint8)bg[0], (Uint8)bg[1], (Uint8)bg[2], (Uint8)bg[3]};

                    // Parse Border Color (Optional)
                    if (p["style"].contains("border_color"))
                    {
                        auto b = p["style"]["border_color"];
                        panel.borderColor = {(Uint8)b[0], (Uint8)b[1], (Uint8)b[2], (Uint8)b[3]};
                    }
                    else
                    {
                        panel.borderColor = {0, 0, 0, 0};
                    }

                    // --- NEW: Parse 9-Slice Image Data ---
                    if (p["style"].contains("border_image"))
                    {
                        panel.borderImage = p["style"]["border_image"];

                        if (p["style"].contains("border_slice"))
                        {
                            auto s = p["style"]["border_slice"];
                            panel.borderSlice = {(int)s[0], (int)s[1], (int)s[2], (int)s[3]};
                        }
                        else
                        {
                            // Default fallback (0 means no slicing, just stretch)
                            panel.borderSlice = {0, 0, 0, 0};
                        }
                    }
                    else
                    {
                        panel.borderImage = ""; // No image
                    }

                    panel.layoutType = p["content_layout"];
                    panelList.push_back(panel);
                }
                scenes[sceneName] = panelList;
            }
            std::cout << "[Layout] Loaded " << scenes.size() << " scenes." << std::endl;
        }
        catch (json::parse_error &e)
        {
            std::cerr << "[Error] Layout JSON: " << e.what() << std::endl;
        }
    }

    void SetActiveScene(const std::string &sceneName)
    {
        if (scenes.count(sceneName))
        {
            currentScene = sceneName;
        }
        else
        {
            std::cerr << "[Warning] Scene not found: " << sceneName << std::endl;
        }
    }

    const std::vector<UIPanel> &GetActivePanels()
    {
        return scenes[currentScene];
    }

    UIPanel GetPanel(const std::string &id)
    {
        for (const auto &p : scenes[currentScene])
        {
            if (p.id == id)
                return p;
        }
        return {};
    }

private:
    std::map<std::string, std::vector<UIPanel>> scenes;
    std::string currentScene = "gameplay";
    LayoutManager() {}
};