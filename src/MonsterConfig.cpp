#include "MonsterConfig.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void MonsterDatabase::LoadDatabase(const std::string &filename)
{
    std::ifstream f(filename);
    if (!f.is_open())
    {
        std::cerr << "[Error] Failed to open monster DB: " << filename << std::endl;
        return;
    }

    try
    {
        json data = json::parse(f);

        // Loop through the "monsters" array in the JSON
        for (const auto &mob : data["monsters"])
        {
            MonsterStats stats;

            // Mandatory Fields
            stats.id = mob["id"];
            stats.name = mob["name"];
            stats.spriteID = mob["sprite_id"];
            stats.hp = mob["hp"];
            stats.damage = mob["damage"];

            // Optional Fields (Safe check)
            if (mob.contains("description"))
            {
                stats.description = mob["description"];
            }

            // Store in map
            database[stats.id] = stats;
            allIDs.push_back(stats.id);
        }
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "[Error] JSON Parse Error: " << e.what() << std::endl;
    }
}

const MonsterStats &MonsterDatabase::GetStats(const std::string &id)
{
    // If ID exists, return it. Otherwise return the first one (safety).
    if (database.count(id))
    {
        return database.at(id);
    }
    std::cerr << "[Warning] Monster ID not found: " << id << std::endl;
    return database.begin()->second;
}

std::string MonsterDatabase::GetRandomID()
{
    if (allIDs.empty())
        return "";
    int index = rand() % allIDs.size();
    return allIDs[index];
}