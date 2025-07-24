#pragma once
#include "../../../Include/Soloud/soloud.h"
#include "../../../Include/Soloud/soloud_wav.h"
#include "../../../Include/Soloud/soloud_wavstream.h"
#include <map>
#include <string>

class SoundManager
{
private:
    static SoundManager* instance;
    SoLoud::Soloud soloud;
    std::map<std::string, SoLoud::Wav*> sounds;

    SoundManager() {}
public:
    ~SoundManager();

    static SoundManager* GetInstance();
    static void Destroy();
    bool Init();
    void LoadSound(const std::string& key, const char* filePath);
    void PlaySound(const std::string& key);
    void CleanUp();
};

