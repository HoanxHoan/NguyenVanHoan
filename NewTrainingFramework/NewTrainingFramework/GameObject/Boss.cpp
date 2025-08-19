#include "Boss.h"
#include <iostream>

Boss::Boss(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)
{
    hp = 5;
    dltime = 0;
    deaddltime = 0;
    death = false;
    isHit = false;
    spawn = false;
}
//void Boss::getObjectList(std::vector<std::shared_ptr<Object>>* O) {
//    others = O;
//}
void Boss::Update(GLfloat deltaTime)
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
    if (deaddltime >= 0.8 && death == true) {
        this->SetVisible(false);
        this->set2Dposition(-10, -10);
    }
    else if (hp <= 0 && death == false) {
        this->Dead();
    }
    else if (dltime >= 0.5 && isHit == true) {
        //printf("%d\n", hp);
        this->endHit();
        hp -= dame;
    }
    SpriteAnimation::Update(deltaTime);
}
void Boss::moveTo(float px, float py, float deltaTime) {
    if (death == true || isHit == true|| spawn == false) { return; }
    this->SetNumFrames(8);
    if (type == 1) {
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(50));
    }
    if (type == 2) {
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(56));
    }
    if (type == 3) {
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(71));
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

    //if (others) {
    //    for (auto& obj : *others) {
    //        if (obj.get() != this && temp.CheckCollision(obj.get())) {
    //            return;
    //        }
    //    }
    //}
    this->x = newX;
    this->y = newY;
    this->SetPosition(Vector3(x, y, 0));
}

void Boss::Draw()
{
    SpriteAnimation::Draw();
}
void Boss::onHit(int count, float damage) {
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
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(51));
        }
        if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(57));
        }
        if (type == 3) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(72));
        }
        this->SetNumFrames(1);
        this->SetCurrentFrame(0);
        isHit = true;
        dltime = 0;
        dame = damage;
    }
}

void Boss::endHit() {
    if (hp >= 1) {
        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(50));
            this->SetNumFrames(8);
        }
        if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(56));
            this->SetNumFrames(8);
        }
        if (type == 3) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(71));
            this->SetNumFrames(8);
        }
        this->SetCurrentFrame(0);
        isHit = false;
        dltime = 0;
    }
}
void Boss::Dead()
{
    if (type == 1) {
        PlayerInventory::GetInstance()->AddItem("green_gem", 3);
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(52));
        this->SetNumFrames(3);
        this->SetFrameTime(0.1);
    }
    if (type == 2) {
        PlayerInventory::GetInstance()->AddItem("red_gem", 3);
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(58));
        this->SetNumFrames(8);
        this->SetFrameTime(0.1);
    }
    if (type == 3) {
        PlayerInventory::GetInstance()->AddItem("blue_gem", 3);
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(73));
        this->SetNumFrames(4);
        this->SetFrameTime(0.3);
    }
    this->SetCurrentFrame(0);
    death = true;
    deaddltime = 0;
}
