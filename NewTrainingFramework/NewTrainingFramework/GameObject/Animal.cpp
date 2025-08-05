#include"Animal.h"
Animal::Animal(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : SpriteAnimation(model, shader, texture, numFrames, currentFrame, numActions, currentAction, frameTime)
{
    currentAction = Idle;
    actionTime = 0;
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
void Animal::Update(GLfloat deltaTime, std::vector<std::shared_ptr<Object>>& others) {
    actionTime += deltaTime;

    if (actionTime >= maxActionTime) {
        ChooseNewAction();
    }

    PerformAction(deltaTime, others);

    SpriteAnimation::Update(deltaTime);
}

void Animal::ChooseNewAction() {
    int randAction = rand() % 5;
    currentAction = static_cast<Action>(randAction);

    actionTime = 0;
    maxActionTime = 2.0f + (rand() % 3000) / 1000.0f; 

    this->SetCurrentAction(currentAction);
    this->SetCurrentFrame(0);
}

void Animal::PerformAction(GLfloat deltaTime, std::vector<std::shared_ptr<Object>>& others) {
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

    if (IsWaterTile(newX, newY))
    {
        return;
    }

    Object temp;
    temp.set2Dposition(newX,newY);
    temp.setSize(this->width, this->height);

    for (auto& obj : others) {
        if (obj.get() != this && temp.CheckCollision(obj.get())) {
            return; 
        }
    }


    this->SetPosition(Vector3(newX,newY,0));
}

void Animal::Draw() {
    SpriteAnimation::Draw();
}
