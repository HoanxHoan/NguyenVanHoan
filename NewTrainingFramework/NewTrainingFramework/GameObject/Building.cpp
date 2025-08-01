#include "Building.h"

Building::Building(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame,
    GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime),
    m_isCollidable(true) 
{
}

void Building::Update(GLfloat deltaTime) {

    SpriteAnimation::Update(deltaTime);

}

void Building::SetIsCollidable(bool collidable) {
    m_isCollidable = collidable;
}

bool Building::IsCollidable() const {
    return m_isCollidable;
}
