#pragma once

enum class StateType
{
    NONE,
    MENU,
    PLAY,
    PAUSE,
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
    virtual StateType GetStateType() const = 0;
};
