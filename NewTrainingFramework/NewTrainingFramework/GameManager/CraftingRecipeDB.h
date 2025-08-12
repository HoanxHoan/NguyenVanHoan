#pragma once
#include <string>
#include <unordered_map>
#include <vector>

struct Ingredient {
    std::string item;
    int amount = 0;
};

struct Recipe {
    std::vector<Ingredient> ingredients;
    std::string station;
};

class CraftingRecipeDB {
private:
    static CraftingRecipeDB* instance;
    

    CraftingRecipeDB() = default; // private constructor

    static void skipSpaces(const std::string& s, size_t& i);
    static std::string removeComments(const std::string& in);
    static std::string parseString(const std::string& s, size_t& i);
    static int parseInt(const std::string& s, size_t& i);
    static void skipValue(const std::string& s, size_t& i);

public:
    std::unordered_map<std::string, Recipe> recipes;
    static CraftingRecipeDB* GetInstance();
    void LoadFromString(const std::string& raw);
    bool LoadFromFile(const std::string& path);
    const Recipe* GetRecipe(const std::string& name) const;
    void PrintAll() const;
};
