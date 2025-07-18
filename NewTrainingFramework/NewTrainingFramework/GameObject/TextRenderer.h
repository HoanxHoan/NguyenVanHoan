#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <string>
#include "../Utilities/utilities.h"
#include "../Shaders.h"
#include "../Globals.h"

struct Character {
    unsigned int TextureID;
    Vector2 Size;
    Vector2 Bearing;
    unsigned int Advance;
};

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    bool Init(const char* fontPath, int fontSize);
    void RenderText(std::string text, float x, float y, float scale, Vector3 color);
private:

    std::map<GLchar, Character> Characters;
    unsigned int VAO, VBO;
    Shaders* textShader;
};
