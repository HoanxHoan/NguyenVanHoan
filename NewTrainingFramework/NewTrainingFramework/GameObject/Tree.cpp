#include "Tree.h"

Tree::Tree(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)
{
    hp = 3;
}
void Tree::CutTree() {
    if (hp >= 1) {
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(17));
        this->SetNumFrames(5);
        this->SetCurrentFrame(0);
        iscut = true;
        dltime = 0;
    }
}
void Tree::EndCutTree() {
    if (hp >= 1) {
        this->SetNumFrames(1);
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(18));
        icut = true;
    }
}
void Tree::Cutted() {
    this->SetNumFrames(1);
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(19));
    this->set2Dposition(this->x, this->y + 20);
    this->setSize(this->width / 4, this->height);
}
void Tree::Update(GLfloat deltaTime)
{
    dltime += deltaTime;
    if(icut == true && dltime >= 0.7)
    {
        hp = hp - 1;
        //printf("%d\n", hp);
        icut = false;
    }
    if (iscut==true  && hp <= 0) { this->Cutted(); }
    else if (iscut && this->GetCurrentFrame()==4) 
    { 
        EndCutTree(); 
    }
    SpriteAnimation::Update(deltaTime);
}

void Tree::Draw()
{
    SpriteAnimation::Draw();
}
