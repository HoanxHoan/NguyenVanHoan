#include "../stdafx.h"
#include "GSIntro.h"
#include "GameStateMachine.h"
#include "GSMenu.h"
#include <iostream>
#define DEG2RAD 0.0174532925199432957f

GSIntro::GSIntro()
{
    introTime = 3.0f;
    elapsedTime = 0.0f;
    Init();
}

GSIntro::~GSIntro() {
    delete obj;
    obj = nullptr;
}

bool GSIntro::Init()
{
    Model* model = new Model();
    model->LoadNFG("../Resources/Models/Sprite2D.nfg");
    Texture* tex = new Texture();
    tex->LoadFromFile("../Resources/Textures/load.tga", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    Shaders* shader = new Shaders();
    shader->Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");
    float px = 480, py = 360, pz = 0, rx = 0, ry = 0, rz = 0, sx = 200, sy = 200, sz = 0;
    Matrix RotationXMatrix;
    Matrix RotationYMatrix;
    Matrix RotationZMatrix;
    RotationXMatrix.SetRotationX(rx * DEG2RAD);
    RotationYMatrix.SetRotationY(ry * DEG2RAD);
    RotationZMatrix.SetRotationZ(rz * DEG2RAD);
    //modelMatrix = scaleMatrix * rotationMatrix * translationMatrix;
    obj = new Object(model, tex, shader);
    obj->translationMatrix.SetTranslation(px, py, pz);
    obj->rotationMatrix = RotationXMatrix * RotationYMatrix * RotationZMatrix;
    obj->scaleMatrix.SetScale(sx, sy, sz);
    float nearPlane = -1, farPlane = 1;
    Camera::GetInstance()->SetNearFar(nearPlane, farPlane);
    std::cout << "Intro Init\n";
    delete model;
    delete tex;
    delete shader;

    return true;
}

void GSIntro::HandleMouseClick(GLint x, GLint y, bool isClick)
{
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
        GameStateMachine::GetInstance()->ChangeState(std::make_unique<GSMenu>()); 
    }
}

void GSIntro::Draw()
{
    if (Scene::GetInstance() && obj)
    {
        obj->Draw();
    }
    std::cout << "Loading..." << "\n";
}

void GSIntro::HandleInput(unsigned char key, bool isPressed)
{
    if (isPressed)
    {
        GameStateMachine::GetInstance()->ChangeState(std::make_unique<GSMenu>());
    }
}
