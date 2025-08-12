#include "Tree.h"

Tree::Tree(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)
{
    hp = 3;
    iscut = false;
    icut = false;
}
void Tree::CutTree() {
    if (hp >= 1 ) {
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
        iscut = false;
        dltime = 0;
        Hpdltime = 0;
    }
}
void Tree::Cutted() {
    this->SetNumFrames(1);
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(19));
    this->set2Dposition(this->x, this->y + 20);
    this->setSize(this->width / 4, this->height);
    iscut = false;
    icut == true;
}
void Tree::Update(GLfloat deltaTime)
{
    Hpdltime += deltaTime;
    dltime += deltaTime;
    if (Hpdltime >= 0.3 && icut == false && hp < 1)
    {
        this->Cutted();
    }
    else  if(iscut == true && dltime >= 0.7)
    {
        //printf("%d\n", hp);
        EndCutTree();
        hp = hp - 1;
    }
   

    SpriteAnimation::Update(deltaTime);
}

void Tree::Draw()
{
    SpriteAnimation::Draw();
}
