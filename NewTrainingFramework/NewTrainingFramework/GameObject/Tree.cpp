#include "Tree.h"

Tree::Tree(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)
{
    hp = 10.0f;
    iscut = false;
    icut = false;
}
void Tree::CutTree(float dame) {
    if (hp >= 1 && iscut == false) {
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(17));
        this->SetNumFrames(5);
        this->SetCurrentFrame(0);
        iscut = true;
        dltime = 0;
        hp = hp - dame;
    }

}
void Tree::EndCutTree() {
    if (hp >= 1) {
        this->SetNumFrames(1);
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(18));       
        dltime = 0;
        Hpdltime = 0;
        //iscut = true;
    }
}
void Tree::Cutted() {
    this->SetNumFrames(1);
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(19));
    this->set2Dposition(this->x, this->y + 20);
    this->setSize(this->width / 4, this->height);
    PlayerInventory::GetInstance()->AddItem("wooden_log", 1);
}
void Tree::Update(GLfloat deltaTime)
{
    Hpdltime += deltaTime;
    dltime += deltaTime;
    if (dltime >= 0.3 && iscut && hp <= 0)
    {
        this->Cutted();
        iscut = false;
        
    }
    else  if(iscut && dltime >= 0.5)
    {
        //printf("%d\n", hp);
        EndCutTree();
        iscut = false;
    }
   

    SpriteAnimation::Update(deltaTime);
}

void Tree::Draw()
{
    SpriteAnimation::Draw();
}
