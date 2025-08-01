#include "Environment.h"

Environment::Environment(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)
{
    m_isCollidable = true;
}
void Environment::CutTree() {
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(17));
    this->SetNumFrames(5);
    this->SetCurrentFrame(0);
}
void Environment::EndCutTree() {
    this->SetNumFrames(1);
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(18));
}
void Environment::Update(GLfloat deltaTime)
{
    SpriteAnimation::Update(deltaTime);
}

void Environment::Draw()
{
    SpriteAnimation::Draw();
}
