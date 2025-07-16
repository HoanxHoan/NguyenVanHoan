#include "../stdafx.h"
#include "GSIntro.h"
#include "GameStateMachine.h"
#include "GSMenu.h"
#include <iostream>
#define DEG2RAD 0.0174532925199432957f
Object* obj;
GSIntro::GSIntro()
{
    introTime = 3.0f; 
    elapsedTime = 0.0f;
    Init();
}

GSIntro::~GSIntro() {}

bool GSIntro::Init()
{
    Model* model = new Model();
    model->LoadNFG("../Resources/Models/Sprite2D.nfg");
    Texture* tex = new Texture();
    tex->LoadFromFile("../Resources/Textures/load.tga", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    Shaders* shader = new Shaders();
    shader->Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");
    float px=0, py=0, pz=0, rx=0, ry=0, rz=0, sx=2, sy=2, sz=2;
    Matrix translationMatrix;
    Matrix rotationMatrix;
    Matrix scaleMatrix;
    Matrix modelMatrix;
    modelMatrix.SetIdentity();
    scaleMatrix.SetScale(sx, sy, sz);
    Matrix RotationXMatrix;
    Matrix RotationYMatrix;
    Matrix RotationZMatrix;
    RotationXMatrix.SetRotationX(rx * DEG2RAD);
    RotationYMatrix.SetRotationY(ry * DEG2RAD);
    RotationZMatrix.SetRotationZ(rz * DEG2RAD);
    rotationMatrix = RotationXMatrix * RotationYMatrix * RotationZMatrix;
    translationMatrix.SetTranslation(px, py, pz);
    obj = new Object(model, tex,shader, modelMatrix, translationMatrix, rotationMatrix, scaleMatrix);
    float nearPlane=-1, farPlane=1, fov=30, speed=1.5;
    Camera::GetInstance()->SetNearFar(nearPlane, farPlane);
    Camera::GetInstance()->SetFOV(fov);
    Camera::GetInstance()->SetSpeed(speed);
    std::cout << "Intro Init\n";
    return true;
}

void GSIntro::Exit()
{
    std::cout << "Intro Exit\n";
}

void GSIntro::Pause() {}
void GSIntro::Resume() {}

void GSIntro::Update(float deltaTime)
{

    if (Scene::GetInstance()->Init())
    {
        GameStateMachine::GetInstance()->ChangeState(new GSMenu());
    }
}

void GSIntro::Draw()
{
    if (Scene::GetInstance())
    {
        obj->Draw();
    }
    std::cout << "Loading..." << "\n";
    Update(0.001);
}

void GSIntro::HandleInput(unsigned char key, bool isPressed)
{
    if (isPressed)
    {
        GameStateMachine::GetInstance()->ChangeState(new GSMenu());
    }
}
