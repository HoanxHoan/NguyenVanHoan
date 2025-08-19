#include "Stone.h"

Stone::Stone(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)
{
    hp = 5;
    iscrush = false;
    icut = false;
}
void Stone::Crush() {
    
    if (hp >= 1 && iscrush == false) {
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
        Hpdltime = 0;
        dltime = 0;
        hp = hp - 1;
    }
}
void Stone::Crushed() {
    this->SetVisible(false);
    this->set2Dposition(-10, -10);
    iscrush = false;
    if (type == 3) {
        PlayerInventory::GetInstance()->AddItem("crystal", 10);
    }
    else PlayerInventory::GetInstance()->AddItem("stone", 2);
}
void Stone::Update(GLfloat deltaTime)
{
    dltime += deltaTime;
    Hpdltime += deltaTime;
    if (iscrush && dltime >= 0.5)
    {
        EndCrush();
        iscrush = false;
    }else if (icut == true && hp <= 1 ) {
        this->Crushed(); 
        icut = false;
    }
    SpriteAnimation::Update(deltaTime);
}

void Stone::Draw()
{
    SpriteAnimation::Draw();
}
