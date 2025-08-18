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
    hitdltime = 0;
    onhit = false;
    hp = 60;
    mp = 60;
    mptime = 0;
    mapWidth = 176;
    mapHeight = 115;
    tileWidth = tileHeight = 16;
    mapPixelWidth = mapWidth * tileWidth;
    mapPixelHeight = mapHeight * tileHeight;
}

bool Player::IsWaterTile(int xPixel, int yPixel) {
    int col = xPixel / tileWidth;
    int row = yPixel / tileHeight;
    int index = row * mapWidth + col;

    if (index >= 0 && index < waterTiles.size()) {
        return waterTiles[index] != 0;
    }
    return false;
}
void Player::getwaterTiles(std::vector<int>& WT) {
    waterTiles = WT;
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
    if (onhit==false) {
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
}
void Player::Crush(int action,int count) {
    if (mp > 0 && action == 0) {
        mp -= 5;
        mptime = 0; 
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
    if (mp > 0 && action == 0) {
        mp -= 5;
        mptime = 0;
        SoundManager::GetInstance()->PlaySoundnoLoop("hitHurt");
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
void Player::onHit(int count,float ex, float ey) {
    float dx = this->x - ex;
    float dy = this->y - ey;
    float len = sqrt(dx * dx + dy * dy);
    if (len != 0) {
        knockbackDirX = dx / len;
        knockbackDirY = dy / len;
    }
    else {
        knockbackDirX = 0;
        knockbackDirY = 0;
    }
    isBeingKnockedBack = true;
    knockbackTime = 0.0f;
        switch (count) {
        case 1: {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(53));
            this->SetNumFrames(4);
            break;
        }
        case 2: {
            this->SetRotation(Vector3(0, 360 * DEG2RAD, 0));
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(54));
            this->SetNumFrames(4);
            break;
        }
        case 3: {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(55));
            this->SetNumFrames(4);
            break;
        }
        case 4: {
            this->SetRotation(Vector3(0, 180 * DEG2RAD, 0));
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(54));
            this->SetNumFrames(4);
            break;
        }
        }
        //if (!isBeingKnockedBack) {
        //    switch (count) {
        //    case 1: knockbackDirX = 0;  knockbackDirY = 1; break;
        //    case 2: knockbackDirX = -1;  knockbackDirY = 0;  break;
        //    case 3: knockbackDirX = 0;  knockbackDirY = -1;  break;
        //    case 4: knockbackDirX = 1; knockbackDirY = 0;  break;
        //    }

        //    isBeingKnockedBack = true;
        //    knockbackTime = 0.0f;
        //}
        this->SetCurrentFrame(0);
        onhit = true;
        hitdltime = 0;
        hp -= 5;
}
Object* Player::GetHitbox(int count) {
    switch (count) {
        case 1: {
            hitbox->height = this->height*3;
            hitbox->width = this->width;
            hitbox->y = this->y - 9;
            hitbox->set2Dposition(this->x, hitbox->y);
            hitbox->setSize(hitbox->width, hitbox->height/3);
            break;
        }
        case 2: {
            hitbox->height = this->height*2 ;
            hitbox->width = this->width*2.5;
            hitbox->x = this->x + 9;
            hitbox->set2Dposition(hitbox->x, this->y);
            hitbox->setSize(hitbox->width, hitbox->height);
            break;
        }
        case 3: {
            hitbox->height = this->height * 3;
            hitbox->width = this->width;
            hitbox->y = this->y + 10;
            hitbox->set2Dposition(this->x, hitbox->y);
            hitbox->setSize(hitbox->width, hitbox->height/3);
            break;
        }
        case 4: {
            hitbox->height = this->height*2 ;
            hitbox->width = this->width * 2.5;
            hitbox->x = this->x - 9;
            hitbox->set2Dposition(hitbox->x, this->y);
            hitbox->setSize(hitbox->width, hitbox->height);
            break;
        }
    }
    return hitbox.get();;
}
void Player::getObjectList(std::vector<std::shared_ptr<Object>>* O) {
    others = O;
}
void Player::Update(GLfloat deltaTime) {
	if (hp > 60) hp = 60;
	if (mp > 60) mp = 60;
	if (hp < 0) hp = 0;
	if (mp < 0) mp = 0;
    mptime += deltaTime;
    if (mp<60 && mptime > 5.0) {
        mp += 5;
        mptime = 0;
    }
    hitdltime += deltaTime;
    if (isBeingKnockedBack) {
        knockbackTime += deltaTime;

        if (knockbackTime < knockbackDuration) {
            float dx = knockbackDirX * knockbackSpeed * deltaTime;
            float dy = knockbackDirY * knockbackSpeed * deltaTime;
            Object temp;
            float newX = this->x + dx;
            float newY = this->y + dy;
            temp.set2Dposition(newX, newY);
            temp.setSize(this->width, this->height);

            bool collided = false;
            for (auto& obj : *others) {
                if (obj.get() != this && temp.CheckCollision(obj.get())) {
                    collided = true;
                    break;
                }
            }

            if (!collided && !IsWaterTile(newX, newY)) {
                this->x += dx;
                this->y += dy;
                this->SetPosition(Vector3(x, y, 0));
            }
        }
        else {
            isBeingKnockedBack = false;
            knockbackDirX = 0;
            knockbackDirY = 0;
        }
    }
    if (hitdltime >= 0.4 && onhit == true) {
        onhit = false;
    }
    SpriteAnimation::Update(deltaTime);

}
