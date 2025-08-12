#include "CraftingRecipeDB.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <iostream>

// ===== Static instance =====
CraftingRecipeDB* CraftingRecipeDB::instance = nullptr;

// ===== Helpers =====
void CraftingRecipeDB::skipSpaces(const std::string& s, size_t& i) {
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
}

std::string CraftingRecipeDB::removeComments(const std::string& in) {
    std::string out;
    size_t i = 0, n = in.size();
    while (i < n) {
        if (i + 1 < n && in[i] == '/' && in[i + 1] == '/') {
            i += 2;
            while (i < n && in[i] != '\n') ++i;
        }
        else if (i + 1 < n && in[i] == '/' && in[i + 1] == '*') {
            i += 2;
            while (i + 1 < n && !(in[i] == '*' && in[i + 1] == '/')) ++i;
            if (i + 1 < n) i += 2;
        }
        else {
            out.push_back(in[i]);
            ++i;
        }
    }
    return out;
}

std::string CraftingRecipeDB::parseString(const std::string& s, size_t& i) {
    skipSpaces(s, i);
    std::string res;
    if (i >= s.size() || s[i] != '"') return res;
    ++i;
    while (i < s.size()) {
        char c = s[i++];
        if (c == '\\' && i < s.size()) {
            res.push_back(s[i]);
            ++i;
        }
        else if (c == '"') {
            break;
        }
        else {
            res.push_back(c);
        }
    }
    return res;
}

int CraftingRecipeDB::parseInt(const std::string& s, size_t& i) {
    skipSpaces(s, i);
    bool neg = false;
    if (i < s.size() && s[i] == '-') { neg = true; ++i; }
    int val = 0;
    while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i]))) {
        val = val * 10 + (s[i] - '0');
        ++i;
    }
    return neg ? -val : val;
}

void CraftingRecipeDB::skipValue(const std::string& s, size_t& i) {
    skipSpaces(s, i);
    if (i >= s.size()) return;
    if (s[i] == '"') {
        parseString(s, i);
    }
    else if (s[i] == '{') {
        int depth = 0;
        do {
            if (s[i] == '{') ++depth;
            else if (s[i] == '}') --depth;
            ++i;
        } while (i < s.size() && depth > 0);
    }
    else if (s[i] == '[') {
        int depth = 0;
        do {
            if (s[i] == '[') ++depth;
            else if (s[i] == ']') --depth;
            ++i;
        } while (i < s.size() && depth > 0);
    }
    else {
        while (i < s.size() && s[i] != ',' && s[i] != ']' && s[i] != '}') ++i;
    }
}

// ===== Singleton methods =====
CraftingRecipeDB* CraftingRecipeDB::GetInstance() {
    if (!instance) {
        instance = new CraftingRecipeDB();
    }
    return instance;
}

void CraftingRecipeDB::LoadFromString(const std::string& raw) {
    recipes.clear();
    std::string s = removeComments(raw);
    size_t i = 0, n = s.size();

    while (i < n) {
        skipSpaces(s, i);
        if (i >= n) break;
        if (s[i] != '"') { ++i; continue; }

        std::string recipeName = parseString(s, i);
        skipSpaces(s, i);
        if (i >= n || s[i] != ':') continue;
        ++i;
        skipSpaces(s, i);
        if (i >= n || s[i] != '{') continue;
        ++i;

        Recipe rec;
        while (i < n) {
            skipSpaces(s, i);
            if (i < n && s[i] == '}') { ++i; break; }
            if (s[i] != '"') { ++i; continue; }

            std::string key = parseString(s, i);
            skipSpaces(s, i);
            if (i < n && s[i] == ':') ++i;
            skipSpaces(s, i);

            if (key == "ingredients") {
                if (i < n && s[i] == '[') ++i;
                while (i < n) {
                    skipSpaces(s, i);
                    if (i < n && s[i] == ']') { ++i; break; }
                    if (i < n && s[i] == '{') {
                        ++i;
                        Ingredient ing;
                        while (i < n) {
                            skipSpaces(s, i);
                            if (i < n && s[i] == '}') { ++i; break; }
                            if (s[i] != '"') { ++i; continue; }
                            std::string ik = parseString(s, i);
                            skipSpaces(s, i);
                            if (i < n && s[i] == ':') ++i;
                            skipSpaces(s, i);
                            if (ik == "item") {
                                ing.item = parseString(s, i);
                            }
                            else if (ik == "amount") {
                                ing.amount = parseInt(s, i);
                            }
                            else {
                                skipValue(s, i);
                            }
                            skipSpaces(s, i);
                            if (i < n && s[i] == ',') ++i;
                        }
                        rec.ingredients.push_back(ing);
                        skipSpaces(s, i);
                        if (i < n && s[i] == ',') ++i;
                    }
                    else {
                        ++i;
                    }
                }
            }
            else if (key == "station") {
                if (i < n && s[i] == '"') {
                    rec.station = parseString(s, i);
                }
                else {
                    skipValue(s, i);
                }
            }
            else {
                skipValue(s, i);
            }
            skipSpaces(s, i);
            if (i < n && s[i] == ',') ++i;
        }
        recipes[recipeName] = rec;
    }
}

bool CraftingRecipeDB::LoadFromFile(const std::string& path) {
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    if (!ifs.is_open()) return false;
    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();
    LoadFromString(content);
    return true;
}

const Recipe* CraftingRecipeDB::GetRecipe(const std::string& name) const {
    auto it = recipes.find(name);
    if (it != recipes.end()) return &it->second;
    return nullptr;
}

void CraftingRecipeDB::PrintAll() const {
    for (auto& p : recipes) {
        std::cout << "Recipe: " << p.first << "\n";
        std::cout << "  Station: " << p.second.station << "\n";
        for (auto& ing : p.second.ingredients) {
            std::cout << "    - " << ing.item << " x" << ing.amount << "\n";
        }
    }
}
