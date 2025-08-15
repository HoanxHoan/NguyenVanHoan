#include "../stdafx.h"
#include "Hmp.h"
#include <unordered_map>
#include "../GameManager/ResourceManager.h"

Hmp::Hmp(Model* model, Texture* texture, Shaders* shader)
    : Object(model, texture, shader)
{
}

Hmp::~Hmp()
{
}
void Hmp::SetPosition(float x, float y)
{
    X = x;
    Y = y;
    this->set2Dposition(x, y);
}

void Hmp::SetSize(float w,float h)
{
    width = w;
    height = h;
    this->setSize(w, h);
}
void Hmp::Update(float x, float y, int hp) {
    SetPosition(x, y);
    if (hp < 0) hp = 0;
    if (hp > 60) hp = 60;
    int hpStep = (hp / 5)+76;
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(hpStep));
}
void Hmp::UpdateMp(float x, float y, int mp) {
    SetPosition(x, y);
    if (mp < 0) mp = 0;
    if (mp > 60) mp = 60;
    int mpStep = (mp / 5) + 89;
    this->SetTexture(ResourceManager::GetInstance()->GetTexture(mpStep));
}