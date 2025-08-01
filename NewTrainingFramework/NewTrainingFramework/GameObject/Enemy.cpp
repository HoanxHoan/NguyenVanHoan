#include "Enemy.h"
#include <iostream>

Enemy::Enemy(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)
{
    m_health = 100; // default HP
}

void Enemy::Update(GLfloat deltaTime)
{
    SpriteAnimation::Update(deltaTime);

    // Logic enemy
    
    // x += 10 * deltaTime;  // dummy movement
}
void Enemy::moveTo(float x, float y){
    this->SetNumFrames(6);
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(22));
    this->SetRotation(Vector3(0, 360 * DEG2RAD, 0));
    float dx = x - this->x;
    float dy = y - this->y;
    float length = std::sqrt(dx * dx + dy * dy);
    if (dx <= 0) { this->SetRotation(Vector3(0, 180 * DEG2RAD, 0)); }
    if (length > 0.01f) { 
        
        dx /= length;
        dy /= length;

        
        float speed = 10.0f;

        
        this->x += dx * speed * 0.016;
        this->y += dy * speed * 0.016;

        set2Dposition(x, y);
    }
}
void Enemy::Draw()
{
    SpriteAnimation::Draw();
}

void Enemy::TakeDamage(int dmg)
{
    m_health -= dmg;
    std::cout << "Enemy takes " << dmg << " damage. Remaining HP: " << m_health << std::endl;
}

bool Enemy::IsDead() const
{
    return m_health <= 0;
}
