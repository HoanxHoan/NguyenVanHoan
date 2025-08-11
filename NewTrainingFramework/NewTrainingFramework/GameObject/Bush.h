#pragma once
#include "SpriteAnimation.h"

class Bush : public SpriteAnimation {
public:
    Bush(Model* model, Shaders* shader, Texture* texture,
        GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime);

    void Update(GLfloat deltaTime) override;
    void Draw() override;
    void Cut();
    void EndCut();
    void Cutted();
    int type;
private:
    bool iscrush;
    float dltime, Hpdltime;
    bool icut;
    int hp;
};

