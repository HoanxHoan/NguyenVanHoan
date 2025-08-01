#pragma once
#include "SpriteAnimation.h"

class Environment : public SpriteAnimation {
public:
    Environment(Model* model, Shaders* shader, Texture* texture,
        GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime);

    void Update(GLfloat deltaTime) override;
    void Draw() override;
    void CutTree();
    void EndCutTree();
    void SetIsCollidable(bool state) { m_isCollidable = state; }
    bool IsCollidable() const { return m_isCollidable; }

private:
    bool m_isCollidable;
};

