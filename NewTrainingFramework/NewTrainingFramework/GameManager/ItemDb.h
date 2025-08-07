#pragma once
#include <string>
#include <unordered_map>

class ItemDB {
public:
    static void LoadDB(const std::string& path);
    static std::unordered_map<std::string, std::string> GetItemData(const std::string& id);

private:
    static std::unordered_map<std::string, std::unordered_map<std::string, std::string>> items;
};
