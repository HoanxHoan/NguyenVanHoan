#pragma once
#include "SpriteAnimation.h"

class Building : public SpriteAnimation {
public:
    Building(Model* model, Shaders* shader, Texture* texture,
             GLint numFrames, GLint currentFrame,
             GLint numActions, GLint currentAction, GLfloat frameTime);

    void Update(GLfloat deltaTime) override;
    void SetIsCollidable(bool collidable);
    bool IsCollidable() const;

private:
    bool m_isCollidable;

};

