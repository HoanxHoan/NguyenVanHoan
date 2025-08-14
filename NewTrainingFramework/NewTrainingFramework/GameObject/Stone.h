#pragma once
#include "SpriteAnimation.h"
#include "../GameManager/PlayerInventory.h"
class Stone : public SpriteAnimation {
public:
    Stone(Model* model, Shaders* shader, Texture* texture,
        GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime);

    void Update(GLfloat deltaTime) override;
    void Draw() override;
    void Crush();
    void EndCrush();
    void Crushed();
    int type;
private:
    bool iscrush;
    float dltime,Hpdltime;
    bool icut;
    int hp;
};

