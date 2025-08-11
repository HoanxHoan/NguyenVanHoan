#include "ItemDB.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

//std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ItemDB::items;

ItemDB* ItemDB::instance = nullptr;

 std::string Trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

 ItemDB* ItemDB::GetInstance() {
     static ItemDB instance;
     return &instance;
 }

 void ItemDB::Destroy()
 {
     if (instance)
     {
         delete instance;
         instance = nullptr;
     }
 }

 ItemDB::ItemDB() {
 }

 ItemDB::~ItemDB() {
 }

void ItemDB::LoadDB(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        printf("Can't open ItemDB: %s\n", path.c_str());
        return;
    }

    std::string line;
    std::string currentItemID;
    std::unordered_map<std::string, std::string> currentItemData;

    while (std::getline(file, line)) {
        line = Trim(line);

        if (line.empty() || line == "{" || line == "},")
            continue;

        // Nếu là dòng kết thúc item: "},"
        if (line.find("}") != std::string::npos) {
            if (!currentItemID.empty()) {
                items[currentItemID] = currentItemData;
                currentItemID.clear();
                currentItemData.clear();
            }
            continue;
        }

        // Nếu là dòng bắt đầu item mới: "item_id": {
        if (line.find(": {") != std::string::npos) {
            // Nếu đang có item chưa lưu thì lưu lại trước
            if (!currentItemID.empty()) {
                items[currentItemID] = currentItemData;
                currentItemData.clear();
            }

            size_t start = line.find('"') + 1;
            size_t end = line.find('"', start);
            currentItemID = line.substr(start, end - start);
            continue;
        }

        // Xử lý dòng key-value trong item
        size_t keyStart = line.find('"') + 1;
        size_t keyEnd = line.find('"', keyStart);
        std::string key = line.substr(keyStart, keyEnd - keyStart);

        size_t valStart = line.find(':', keyEnd) + 1;
        std::string val = Trim(line.substr(valStart));

        if (!val.empty() && val.back() == ',')
            val.pop_back();

        if (!val.empty() && val.front() == '"')
            val = val.substr(1, val.length() - 2);

        currentItemData[key] = val;
    }

    // Lưu item cuối cùng nếu còn
    if (!currentItemID.empty()) {
        std::cout << "Inserting item ID: [" << currentItemID << "]\n";
        items[currentItemID] = currentItemData;
    }

    file.close();
    PrintAllItems();
}

std::unordered_map<std::string, std::string> ItemDB::GetItemData(const std::string& id)
{
    if (items.find(id) != items.end()) {
		printf("Founded item: %s\n", id.c_str());
        return items[id];
    }

    return {};
}

void ItemDB::PrintAllItems()
{
    for (const auto& pair : items) {
        std::cout << "Item ID: " << pair.first << "\n";
        for (const auto& field : pair.second) {
            std::cout << "  " << field.first << ": " << field.second << "\n";
        }
        std::cout << "----------------------\n";
    }
}