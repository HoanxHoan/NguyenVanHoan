#include "Stone.h"

Stone::Stone(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)
{
    hp = 7;
}
void Stone::Crush() {
    if (hp >= 1) {
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(31));
        this->SetNumFrames(1);
        this->SetCurrentFrame(0);
        iscrush = true;
        dltime = 0;
    }
}
void Stone::EndCrush() {
    if (hp >= 1) {
        this->SetNumFrames(1);
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(28));
        icut = true;
        iscrush = false;
        dltime = 0;
    }
}
void Stone::Crushed() {
    this->SetVisible(false);
    this->set2Dposition(-10, -10);
    iscrush = false;
}
void Stone::Update(GLfloat deltaTime)
{
    dltime += deltaTime;
    if (icut == true && dltime >= 1.0)
    {
        hp = hp - 1;
        icut = false;
    }
    if (iscrush == true && hp <= 0) { this->Crushed(); }
    else if (iscrush && dltime>=0.8)
    {
        EndCrush();
    }
    SpriteAnimation::Update(deltaTime);
}

void Stone::Draw()
{
    SpriteAnimation::Draw();
}
