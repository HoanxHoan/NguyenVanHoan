#include"Animal.h"
Animal::Animal(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)
{
    currentAction = Idle;
    actionTime = 0;
    dltime = 0;
    deaddltime = 0;
    hp = 2;
    isdead = false;
    isHit = false;
    maxActionTime = 2.0f + (rand() % 3000) / 1000.0f; 
    speed = 50.0f;
    mapWidth = 176;
    mapHeight = 115;
    tileWidth = tileHeight = 16;
    mapPixelWidth = mapWidth * tileWidth;
    mapPixelHeight = mapHeight * tileHeight;
}
bool Animal::IsWaterTile(int xPixel, int yPixel) {
    int col = xPixel / tileWidth;
    int row = yPixel / tileHeight;
    int index = row * mapWidth + col;

    if (index >= 0 && index < waterTiles.size()) {
        return waterTiles[index] != 0;
    }
    return false;
}
void Animal::getObjectList(std::vector<std::shared_ptr<Object>>* O) {
    others = O;
}
void Animal::getwaterTiles(std::vector<int>& WT) {
    waterTiles = WT;
}
void Animal::Update(GLfloat deltaTime) {
    actionTime += deltaTime;
    dltime += deltaTime;
    deaddltime += deltaTime;
    if (deaddltime >= 0.3 && isdead ==true) {
        this->SetVisible(false);
        this->set2Dposition(-10, -10);
    }
    else if (hp < 1 && isdead == false) {
        this->dead();
    }
    else if (dltime >= 0.5 && isHit == true) {
        hp -= 1;
        //printf("%d\n", hp);
        this->endHit();
    }
    else if (isdead == false && isHit == false) {
        if (actionTime >= maxActionTime) {
            ChooseNewAction();
        }
        PerformAction(deltaTime);
    }
    SpriteAnimation::Update(deltaTime);
}

void Animal::ChooseNewAction() {
    int randAction = rand() % 5;
    currentAction = static_cast<Action>(randAction);

    actionTime = 0;
    maxActionTime = 2.5f + (rand() % 1000) / 1000.0f;
    if(currentAction == Idle)
    {
        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(39));
        }
        if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(43));
        }
    }
    else if (currentAction == MoveLeft)
    {
        this->SetRotation(Vector3(0, 180 * DEG2RAD, 0));
        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(40));
        }
        if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(44));
        }
    }
    else if (currentAction == MoveRight)
    {
        this->SetRotation(Vector3(0, 360* DEG2RAD,0));
        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(40));
        }
        if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(44));
        }
    }
    else if (currentAction == MoveUp)
    {
        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(40));
        }
        if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(44));
        }
    }
    else if (currentAction == MoveDown)
    {
        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(40));
        }
        if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(44));
        }
    }
    
    this->SetCurrentFrame(0);
}

void Animal::PerformAction(GLfloat deltaTime) {
    float newX = this->x;
    float newY = this->y; 

    switch (currentAction) {
    case MoveLeft:
        newX -= speed * deltaTime;
        break;
    case MoveRight:
        newX += speed * deltaTime;
        break;
    case MoveUp:
        newY += speed * deltaTime;
        break;
    case MoveDown:
        newY -= speed * deltaTime;
        break;
    case Idle:
    default:
        return;
    }

    if (IsWaterTile(newX, newY)|| IsWaterTile(newX, newY+1)|| IsWaterTile(newX+1, newY)|| IsWaterTile(newX, newY-1)|| IsWaterTile(newX-1, newY))
    {
        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(39));
        }
        if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(43));
        }
        currentAction = Idle;
        return;
    }

    Object temp;
    temp.set2Dposition(newX,newY);
    temp.setSize(this->width, this->height);

    for (auto& obj : *others) {
        if (obj.get() != this && temp.CheckCollisionanimal(obj.get())) {
            if (type == 1) {
                this->SetTexture(ResourceManager::GetInstance()->GetTexture(39));
            }
            if (type == 2) {
                this->SetTexture(ResourceManager::GetInstance()->GetTexture(43));
            }
            currentAction = Idle;
            return; 
        }
    }
   

    this->SetPosition(Vector3(newX,newY,0));
}
void Animal::onHit() {
    if (hp >=1) {
        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(42));
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
void Animal::endHit() {
    if (hp >= 1) {
        if (type == 1) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(39));
        }
        if (type == 2) {
            this->SetTexture(ResourceManager::GetInstance()->GetTexture(43));
        }
        this->SetNumFrames(4);
        this->SetCurrentFrame(0);
        isHit = false;
        dltime = 0;
    }
}
void Animal::dead() {
    if (type == 1) {
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(41));
        PlayerInventory::GetInstance()->AddItem("cooked_beef", 2);
    }
    if (type == 2) {
        this->SetTexture(ResourceManager::GetInstance()->GetTexture(45));
        PlayerInventory::GetInstance()->AddItem("bacon", 2);
    }
    this->SetNumFrames(4);
    this->SetCurrentFrame(0);
    isdead = true;  
    deaddltime = 0;
}
void Animal::Draw() {
    SpriteAnimation::Draw();
}
