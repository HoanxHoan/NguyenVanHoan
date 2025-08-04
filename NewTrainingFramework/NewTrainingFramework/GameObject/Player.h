#pragma once
#include "SpriteAnimation.h"

class Player : public SpriteAnimation {
public:
    Player(Model* model, Shaders* shader, Texture* texture,
        GLint numFrames, GLint currentFrame,
        GLint numActions, GLint currentAction, GLfloat frameTime);

    void Update(GLfloat deltaTime) override;
    void MoveUp(float value, bool col);
    void MoveRight(float value, bool col);
    void MoveDown(float value, bool col);
    void MoveLeft(float value, bool col);
    void Idle(int count);
    void Crush(int action, int count);
    void Slice(int action, int count);
    void Dead();
    Object* GetHitbox(int count);
    ///void SetMapBoundary(float maxX, float maxY);
    bool IsWaterTile(int xPixel, int yPixel);
    //float clamp(float value, float min, float max);
    int m_maxX, m_maxY;
    std::shared_ptr<Object>hitbox;
};

