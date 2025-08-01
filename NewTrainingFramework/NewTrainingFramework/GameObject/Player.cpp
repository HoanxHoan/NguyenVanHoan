#include "Player.h"
#include <algorithm> 

Player::Player(Model* model, Shaders* shader, Texture* texture,
               GLint numFrames, GLint currentFrame,
               GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)     
{
}
float Player:: clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
void Player::SetMapBoundary(float maxX,float maxY) {
    m_maxX = maxX;
    m_maxY = maxY;
}

void Player::Move(float dx, float dy) {
    x += dx;
    y += dy;

    x = clamp(x, 0.0f, (float)(m_maxX - this->width));
    y = clamp(y, 0.0f, (float)(m_maxY - this->height));

    set2Dposition(x, y);
}

void Player::Update(GLfloat deltaTime) {
    SpriteAnimation::Update(deltaTime);

}
