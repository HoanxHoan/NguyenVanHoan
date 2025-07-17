#include "../stdafx.h"
#include "GameButton.h"

GameButton::GameButton(Model* model, Texture* texture, Shaders* shader)
    : Object(model, texture, shader), m_pBtClick(nullptr), m_isHolding(false)
{
}

GameButton::~GameButton()
{
}
void GameButton::SetPosition(float x, float y)
{
    posX = x;
    posY = y;
    
}

void GameButton::SetSize(float w, float h)
{
    width = w;
    height = h;
}
void GameButton::SetOnClick(void (*func)())
{
    m_pBtClick = func;
}

bool GameButton::HandleTouchEvents(GLint x, GLint y, bool bIsPressed)
{
    bool isHandled = false;
    if (bIsPressed)
    {
        if ((x > posX - width / 2) && (x < posX + width / 2)
            && (y > posY - height / 2) && (y < posY + height / 2))
        {
            m_isHolding = true;
        }
    }
    else
    {
      if ((x > posX - width / 2) && (x < posX + width / 2)
          && (y > posY - height / 2) && (y < posY + height / 2)
          && m_isHolding == true)
      {
                m_pBtClick();
                isHandled = true;
      }
          m_isHolding = false;
    }
    return isHandled;
}

bool GameButton::IsHolding()
{
    return m_isHolding;
}
