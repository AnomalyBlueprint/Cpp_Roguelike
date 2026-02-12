#pragma once
#include <string>
#include <map>
#include <vector>
#include "json.hpp" // Ensure this is in your include folder

// 1. The Data Structure (Matches your JSON)
struct MonsterStats
{
    std::string id;
    std::string name;
    int spriteID;
    int hp;
    int damage;
    std::string description;
};

// 2. The Database Class (Singleton)
class MonsterDatabase
{
public:
    // Get the global instance
    static MonsterDatabase &Get()
    {
        static MonsterDatabase instance;
        return instance;
    }

    // Load data from disk
    void LoadDatabase(const std::string &filename);

    // Retrieve stats by ID (e.g., "rat_sewer")
    const MonsterStats &GetStats(const std::string &id);

    // Get a random monster ID (for spawning)
    std::string GetRandomID();

private:
    std::map<std::string, MonsterStats> database;
    std::vector<std::string> allIDs; // For random selection
};