#pragma once
#include "Object.h"

class GameButton : public Object
{
public:
    GameButton() : Object(), m_pBtClick(nullptr), m_isHolding(false){}
    GameButton(Model* model, Texture* texture, Shaders* shade);
    ~GameButton();

    void SetOnClick(void (*m_pBtClick)());

    bool HandleTouchEvents(GLint x, GLint y,bool bIsPressed); 
    bool IsHolding(); 
    void SetPosition(float x, float y);
    void SetSize(float width, float height);
private:
    void (*m_pBtClick)();
    bool m_isHolding;
    float posX, posY; 
    float width, height; 
};
