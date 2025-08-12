#include "Bush.h"

Bush::Bush(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)
{
    hp = 2;
}
void Bush::Cut() {

    if (hp >= 1) {
        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(37));
            this->SetNumFrames(1);
            this->SetCurrentFrame(0);
            iscrush = true;
            dltime = 0;
        }
        else if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(38));
            this->SetNumFrames(1);
            this->SetCurrentFrame(0);
            iscrush = true;
            dltime = 0;
        }
    }
}
void Bush::EndCut() {
    if (hp >= 1) {
        if (type == 1) {
            this->SetNumFrames(1);
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(35));
            icut = true;
            iscrush = false;
            Hpdltime = 0;
            dltime = 0;
        }
        else if (type == 2) {
            this->SetNumFrames(1);
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(36));
            icut = true;
            iscrush = false;
            Hpdltime = 0;
            dltime = 0;
        }
    }
}
void Bush::Cutted() {
    this->SetVisible(false);
    this->set2Dposition(-10, -10);
    iscrush = false;
}
void Bush::Update(GLfloat deltaTime)
{
    dltime += deltaTime;
    Hpdltime += deltaTime;
    if (Hpdltime >= 0.1 && hp <= 1) { this->Cutted();}
    else if (iscrush && dltime >= 0.1)
    {
        EndCut();
        hp = hp - 1;
    }
    SpriteAnimation::Update(deltaTime);
}

void Bush::Draw()
{
    SpriteAnimation::Draw();
}
