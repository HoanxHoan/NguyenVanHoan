#pragma once
#include <string>
#include <unordered_map>

class ItemDB {
public:
     static ItemDB* instance;
     ItemDB();
     ~ItemDB();
     static ItemDB* GetInstance();
	 static void Destroy();
        // Không cho phép copy/move
     ItemDB(const ItemDB&) = delete;
     ItemDB& operator=(const ItemDB&) = delete;

     void LoadDB(const std::string& path);
     std::unordered_map<std::string, std::string> GetItemData(const std::string& id);
     void PrintAllItems();
     std::unordered_map<std::string, std::unordered_map<std::string, std::string>> items;

     int GetStackSize(const std::string& id);
private:
};
