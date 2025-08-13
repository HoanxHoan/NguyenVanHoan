#include "Stone.h"

Stone::Stone(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)
{
    hp = 5;
}
void Stone::Crush() {
    
    if (hp >= 1) {
        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(31));
            this->SetNumFrames(1);
            this->SetCurrentFrame(0);
        }
        else if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(32));
            this->SetNumFrames(1);
            this->SetCurrentFrame(0);
        }
        else if (type == 3) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(75));
            this->SetNumFrames(1);
            this->SetCurrentFrame(0);
        }
        iscrush = true;
        dltime = 0;
    }
}
void Stone::EndCrush() {
    if (hp >= 1) {
        if (type == 1) {
            this->SetNumFrames(1);
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(28));
        }else if(type ==2 ){
            this->SetNumFrames(1);
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(29));
        }
        else if (type == 3) {
            this->SetNumFrames(1);
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(74));
        }
        icut = true;
        iscrush = false;
        Hpdltime = 0;
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
    Hpdltime += deltaTime;
    if (icut == true && Hpdltime >= 0.5)
    {
        icut = false;
    }
    if (iscrush == true && hp <= 1) { this->Crushed(); }
    else if (iscrush && dltime>=0.1)
    {
        hp = hp - 1;
        EndCrush();
    }
    SpriteAnimation::Update(deltaTime);
}

void Stone::Draw()
{
    SpriteAnimation::Draw();
}
