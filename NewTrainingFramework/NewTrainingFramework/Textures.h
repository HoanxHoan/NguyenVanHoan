
#include <GLES3/gl3.h>

char* LoadTGA(const char* filePath, int* width, int* height, int* bpp);

class Texture
{
public:

    GLuint textureID;
    int width, height, bpp;

    // Constructor
    Texture();

    // Destructor
    ~Texture();

    // Load texture 
    bool LoadFromFile(const char* filePath);

    // Bind texture 
    void Bind(GLuint unit = 0);

    // Unbind texture
    void Unbind();
};
