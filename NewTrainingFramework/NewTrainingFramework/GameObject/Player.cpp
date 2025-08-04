#include "Player.h"
#include <algorithm> 

Player::Player(Model* model, Shaders* shader, Texture* texture,
               GLint numFrames, GLint currentFrame,
               GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)     
{
    hitbox = std::make_shared<Object>();
    hitbox->objModel = ResourceManager::GetInstance()->GetModel(2);
    hitbox->objTex = ResourceManager::GetInstance()->GetTexture(3);
    hitbox->objShader = ResourceManager::GetInstance()->GetShader(0);
}


void Player::MoveUp(float value,bool col) {
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(5));
    this->SetNumFrames(6);
    if (!col) {
        this->y = value;
        this->SetPosition(Vector3(this->x, this->y, 0));
    }
}
void Player::MoveRight(float value, bool col) {
    this->SetRotation(Vector3(0, 360 * DEG2RAD, 0));
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(0));
    this->SetNumFrames(6);
    if (!col) {
        this->x = value;
        this->SetPosition(Vector3(this->x, this->y, 0));
    }
}
void Player::MoveDown(float value, bool col) {
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(4));
    this->SetNumFrames(6);
    if (!col) {
        this->y = value;
        this->SetPosition(Vector3(this->x, this->y, 0));
    }
}
void Player::MoveLeft(float value, bool col) {
    this->SetRotation(Vector3(0, 180 * DEG2RAD, 0));
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(0));
    this->SetNumFrames(6);
    if (!col) {
        this->x = value;
        this->SetPosition(Vector3(this->x, this->y, 0));
    }
}
void Player::Idle(int count) {
    this->SetNumFrames(4);
    switch (count) {
    case 1:this->SetTexture(ResourceManager::GetInstance()->GetTexture(9)); break;
    case 2: {
        this->SetRotation(Vector3(0, 360 * DEG2RAD, 0));
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(7));
        break;
    }
    case 3:this->SetTexture(ResourceManager::GetInstance()->GetTexture(8)); break;
    case 4: {
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(7));
        this->SetRotation(Vector3(0, 180 * DEG2RAD, 0));
        break;
    }
    }
}
void Player::Crush(int action,int count) {
    if (action == 0) {
        switch (count) {
        case 1: {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(12));
            this->SetNumFrames(8);
            this->SetCurrentFrame(0);
            
            break;
        }
        case 2: {
            this->SetRotation(Vector3(0, 360 * DEG2RAD, 0));
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(10));
            this->SetNumFrames(8);
            this->SetCurrentFrame(0);
            
            break;
        }
        case 3: {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(11));
            this->SetNumFrames(8);
            this->SetCurrentFrame(0);
            
            break;
        }
        case 4: {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(10));
            this->SetNumFrames(8);
            this->SetCurrentFrame(0);
            this->SetRotation(Vector3(0, 180 * DEG2RAD, 0));
            
            break;
        }
        }
    }
}
void Player::Slice(int action, int count) {
    if (action == 0) {
        switch (count) {
        case 1: {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(25));
            this->SetNumFrames(8);
            this->SetCurrentFrame(0);

            break;
        }
        case 2: {
            this->SetRotation(Vector3(0, 360 * DEG2RAD, 0));
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(23));
            this->SetNumFrames(8);
            this->SetCurrentFrame(0);

            break;
        }
        case 3: {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(24));
            this->SetNumFrames(8);
            this->SetCurrentFrame(0);

            break;
        }
        case 4: {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(23));
            this->SetNumFrames(8);
            this->SetCurrentFrame(0);
            this->SetRotation(Vector3(0, 180 * DEG2RAD, 0));

            break;
        }
        }
    }
}
void Player::Dead() {
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(23));
    this->SetNumFrames(8);
}
Object* Player::GetHitbox(int count) {
    switch (count) {
        case 1: {
            hitbox->height = this->height*3;
            hitbox->width = this->width/3;
            hitbox->y = this->y - 6;
            hitbox->set2Dposition(this->x, hitbox->y);
            hitbox->setSize(hitbox->width, hitbox->height);
            break;
        }
        case 2: {
            hitbox->height = this->height * 2;
            hitbox->width = this->width;
            hitbox->x = this->x + 4;
            hitbox->set2Dposition(hitbox->x, this->y);
            hitbox->setSize(hitbox->width, hitbox->height);
            break;
        }
        case 3: {
            hitbox->height = this->height * 3;
            hitbox->width = this->width/3;
            hitbox->y = this->y + 6;
            hitbox->set2Dposition(this->x, hitbox->y);
            hitbox->setSize(hitbox->width, hitbox->height);
            break;
        }
        case 4: {
            hitbox->height = this->height * 2;
            hitbox->width = this->width;
            hitbox->x = this->x - 4 ;
            hitbox->set2Dposition(hitbox->x, this->y);
            hitbox->setSize(hitbox->width, hitbox->height);
            break;
        }
    }
    return hitbox.get();;
}
void Player::Update(GLfloat deltaTime) {
    SpriteAnimation::Update(deltaTime);

}
