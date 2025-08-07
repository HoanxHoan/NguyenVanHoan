#include "Enemy.h"
#include <iostream>

Enemy::Enemy(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)
{
    hp = 2; 
    dltime = 0;
    deaddltime = 0;
    death = false;
    isHit = false;
}
void Enemy::getObjectList(std::vector<std::shared_ptr<Object>>* O) {
    others = O;
}
void Enemy::Update(GLfloat deltaTime)
{
    dltime += deltaTime;
    deaddltime += deltaTime;
    if (deaddltime >= 0.3 && death == true) {
        this->SetVisible(false);
        this->set2Dposition(-10, -10);
    }
    else if (hp < 1 && death == false) {
        this->Dead();
    }
    else if (dltime >= 0.6 && isHit == true) {
        hp -= 1;
        //printf("%d\n", hp);
        this->endHit();
    }
    SpriteAnimation::Update(deltaTime);
}
void Enemy::moveTo(float px, float py, float deltaTime) {
    if (death == true||isHit==true) { return; }
    this->SetNumFrames(6);
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(22));

    float dx = px - this->x;
    float dy = py - this->y;
    float length = std::sqrt(dx * dx + dy * dy);

    if (length < 0.01f) return;

    if (dx <= 0) this->SetRotation(Vector3(0, 180 * DEG2RAD, 0));
    else this->SetRotation(Vector3(0, 0, 0));

    dx /= length;
    dy /= length;

    float speed = 30.0f;
    float newX = this->x + dx * speed * deltaTime;
    float newY = this->y + dy * speed * deltaTime;

    Object temp;
    temp.set2Dposition(newX, newY);
    temp.setSize(this->width, this->height);

    if (others) {
        for (auto& obj : *others) {
            if (obj.get() != this && temp.CheckCollision(obj.get())) {
                return; 
            }
        }
    }
    this->x = newX;
    this->y = newY;
    this->SetPosition(Vector3(x, y, 0));
}

void Enemy::Draw()
{
    SpriteAnimation::Draw();
}
void Enemy::onHit(int count) {
    if (hp >= 1) {
        switch (count) {
        case 1: {
            this->y -= 10;
            this->SetPosition(Vector3(x, y, 0));
            break;
        }
        case 2: {
            this->x += 10;
            this->SetPosition(Vector3(x, y, 0));
            break;
        }
        case 3: {
            this->y += 10;
            this->SetPosition(Vector3(x, y, 0));
            break;
        }
        case 4: {
            this->x -= 10;
            this->SetPosition(Vector3(x, y, 0));
            break;
        }
        }
        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(21));
        }
        if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(46));
        }
        this->SetNumFrames(1);
        this->SetCurrentFrame(0);
        isHit = true;
        dltime = 0;
    }
}
void Enemy::endHit() {
    if (hp >= 1) {
        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(22));
        }
        if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(43));
        }
        this->SetNumFrames(6);
        this->SetCurrentFrame(0);
        isHit = false;
        dltime = 0;
    }
}
void Enemy::Dead()
{
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(26));
    this->SetNumFrames(6);
    this->SetCurrentFrame(0);
    death = true;
    deaddltime = 0;
}
