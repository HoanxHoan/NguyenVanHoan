#pragma once
#include "SpriteAnimation.h"

class Enemy : public SpriteAnimation {
public:
    Enemy(Model* model, Shaders* shader, Texture* texture,
        GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime);

    void Update(GLfloat deltaTime) override;
    void Draw() override;

    void SetHealth(int hp) { m_health = hp; }
    int GetHealth() const { return m_health; }
    void moveTo(float x,float y);
    void TakeDamage(int dmg);
    bool IsDead() const;

private:
    int m_health;
};

