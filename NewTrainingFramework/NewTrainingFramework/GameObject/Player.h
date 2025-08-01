#pragma once
#include "SpriteAnimation.h"

class Player : public SpriteAnimation {
public:
    Player(Model* model, Shaders* shader, Texture* texture,
        GLint numFrames, GLint currentFrame,
        GLint numActions, GLint currentAction, GLfloat frameTime);

    void Update(GLfloat deltaTime) override;
    void Move(float dx, float dy);
    void SetMapBoundary(float maxX, float maxY);
    bool IsWaterTile(int xPixel, int yPixel);
    float clamp(float value, float min, float max);
private :
    int m_maxX, m_maxY;

};

