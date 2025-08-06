#include "ItemDB.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ItemDB::items;

static std::string Trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

void ItemDB::LoadDB(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        printf("Không thể mở file ItemDB: %s\n", path.c_str());
        return;
    }

    std::string line;
    std::string currentItemID;
    std::unordered_map<std::string, std::string> currentItemData;

    while (std::getline(file, line)) {
        line = Trim(line);

        if (line.empty() || line == "{" || line == "},")
            continue;

        if (line.find("}") != std::string::npos) {
            if (!currentItemID.empty()) {
                items[currentItemID] = currentItemData;
                currentItemID.clear();
                currentItemData.clear();
            }
            continue;
        }

        // ID: "wood_log": {
        if (line.find(": {") != std::string::npos) {
            size_t start = line.find('"') + 1;
            size_t end = line.find('"', start);
            currentItemID = line.substr(start, end - start);
            continue;
        }

        // Field: "type": "material",
        size_t keyStart = line.find('"') + 1;
        size_t keyEnd = line.find('"', keyStart);
        std::string key = line.substr(keyStart, keyEnd - keyStart);

        size_t valStart = line.find(':', keyEnd) + 1;
        std::string val = Trim(line.substr(valStart));

        // Remove comma
        if (!val.empty() && val.back() == ',')
            val.pop_back();

        // Remove quotes if any
        if (!val.empty() && val.front() == '"')
            val = val.substr(1, val.length() - 2);

        currentItemData[key] = val;
    }

    file.close();
}

std::unordered_map<std::string, std::string> ItemDB::GetItemData(const std::string& id)
{
    if (items.find(id) != items.end()) {
        return items[id];
    }
    return {};
}
