#pragma once
#include <GLES3/gl3.h>
enum class StateType
{
    NONE,
    MENU,
    PLAY,
    PAUSE,
    INTRO,
};

class GameStateBase
{
public:
    GameStateBase() {}
    virtual ~GameStateBase() {}

    virtual bool Init() = 0;
    virtual void Exit() = 0;

    virtual void Pause() = 0;
    virtual void Resume() = 0;

    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    virtual void HandleInput(unsigned char key, bool isPressed) = 0;
    virtual void HandleMouseClick(GLint x, GLint y, bool isClick) = 0;
    virtual StateType GetStateType() const = 0;
};
