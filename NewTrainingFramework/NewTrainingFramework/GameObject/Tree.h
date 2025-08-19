#pragma once
#include "SpriteAnimation.h"
#include "../GameManager/PlayerInventory.h"
class Tree : public SpriteAnimation {
public:
    Tree(Model* model, Shaders* shader, Texture* texture,
        GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime);

    void Update(GLfloat deltaTime) override;
    void Draw() override;
    void CutTree(float dame);
    void EndCutTree();
    void Cutted();
private:
    bool iscut;
    float dltime, Hpdltime;
    bool icut,iscutted;
    float hp;
};

