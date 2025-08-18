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
    if (isBeingKnockedBack) {
        knockbackTime += deltaTime;

        if (knockbackTime < knockbackDuration) {
            float dx = knockbackDirX * knockbackSpeed * deltaTime;
            float dy = knockbackDirY * knockbackSpeed * deltaTime;

            this->x += dx;
            this->y += dy;
            this->SetPosition(Vector3(x, y, 0));
        }
        else {
            isBeingKnockedBack = false;
            knockbackDirX = 0;
            knockbackDirY = 0;
        }
    }
    dltime += deltaTime;
    deaddltime += deltaTime;
    if (deaddltime >= 0.3 && death == true) {
        if(type == 1) {
            PlayerInventory::GetInstance()->AddItem("goblin_ear", 2);
        } else if (type == 2) {
            PlayerInventory::GetInstance()->AddItem("bone", 2);
		}
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

    if (type == 1) {
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(22));
    }
    if (type == 2) {
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(47));
    }

    float dx = px - this->x;
    float dy = py - this->y;
    float length = std::sqrt(dx * dx + dy * dy);

    if (length < 0.01f) return;

    if (dx <= 0) this->SetRotation(Vector3(0, 180 * DEG2RAD, 0));
    else this->SetRotation(Vector3(0, 0, 0));

    dx /= length;
    dy /= length;

    float speed = 10.0f;
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
    if (hp >= 1 && !isBeingKnockedBack) {
        switch (count) {
        case 1: knockbackDirX = 0;  knockbackDirY = -1; break;
        case 2: knockbackDirX = 1;  knockbackDirY = 0;  break;
        case 3: knockbackDirX = 0;  knockbackDirY = 1;  break;
        case 4: knockbackDirX = -1; knockbackDirY = 0;  break;
        }

        isBeingKnockedBack = true;
        knockbackTime = 0.0f;

        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(21));
        }
        if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(48));
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
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(47));
        }
        this->SetNumFrames(6);
        this->SetCurrentFrame(0);
        isHit = false;
        dltime = 0;
    }
}
void Enemy::Dead()
{
    if (type == 1) {
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(26));
    }
    if (type == 2) {
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(49));
    }
    this->SetNumFrames(6);
    this->SetCurrentFrame(0);
    death = true;
    deaddltime = 0;
}
