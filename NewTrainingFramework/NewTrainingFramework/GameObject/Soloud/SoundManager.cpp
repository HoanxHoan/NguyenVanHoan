#include "SoundManager.h"
#include <iostream>
SoundManager* SoundManager::instance = nullptr;

SoundManager::~SoundManager()
{
    CleanUp();
}

SoundManager* SoundManager::GetInstance()
{
    if (!instance)
        instance = new SoundManager();
    return instance;
}
void SoundManager::Destroy()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}
bool SoundManager::Init()
{
    if (soloud.init() != 0)
    {
        std::cout << "SoLoud init failed" << std::endl;
        return false;
    }
    return true;
}

void SoundManager::LoadSound(const std::string& key, const char* filePath)
{
    SoLoud::Wav* wav = new SoLoud::Wav();
    if (wav->load(filePath) == 0)
    {
        sounds[key] = wav;
        std::cout << "Loaded sound: " << filePath << std::endl;
    }
    else
    {
        std::cout << "Failed to load sound: " << filePath << std::endl;
        delete wav;
    }
}

void SoundManager::PlaySound(const std::string& key)
{
    auto it = sounds.find(key);
    if (it != sounds.end())
        soloud.play(*it->second);
    else
        std::cout << "Sound not found: " << key << std::endl;
}

void SoundManager::CleanUp()
{
    soloud.stopAll(); 
    for (auto& s : sounds)
        delete s.second;
    sounds.clear();
    soloud.deinit();
}
