// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include <conio.h>
#include <cstdio>   // fopen_s, fscanf, fclose
#include <iostream>
#include "GameObject/Textures.h"
#include "GameManager/Scene.h"
#include "GameManager/GameStateMachine.h"
#include "GameManager/GSIntro.h"
#define DEG2RAD 0.0174532925199432957f
float deltaTime = 0.016f;
bool keyStates[256];
int id = 6;
//Scene* scene = nullptr;
int Init ( ESContext *esContext )
{

	GameStateMachine::GetInstance()->ChangeState(new GSIntro());
	return 0;

}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GameStateMachine::GetInstance()->Draw();
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

}

void Update ( ESContext *esContext, float deltaTime )
{
	GameStateMachine::GetInstance()->CurrentState()->Update(deltaTime);
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if (GameStateMachine::GetInstance()->CurrentState())
	{
		GameStateMachine::GetInstance()->CurrentState()->HandleInput(key, bIsPressed);
	}

	//keyStates[key] = bIsPressed;
}
void MouseClick(ESContext* esContext, GLint x,GLint y, bool isClick)
{
	if (GameStateMachine::GetInstance()->CurrentState())
	{
		GameStateMachine::GetInstance()->CurrentState()->HandleMouseClick(x, y, isClick);
	}
	if (isClick) {
		printf("x:%d,y:%d\n", x, y);
	}
}
void OnMouseMove(ESContext* esContext, GLint x, GLint y)
{
	
}
void CleanUp()
{
	Scene::Destroy();
	GameStateMachine::Destroy();
	ResourceManager::Destroy();
	Camera::GetInstance()->Destroy();
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Woman1", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	/*if ( Init ( &esContext ) != 0 )
		return 0;*/
	Init(&esContext);
	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterMouseFunc(&esContext, MouseClick);
	esRegisterMouseMoveFunc(&esContext, OnMouseMove);
	esMainLoop ( &esContext );
	printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}

