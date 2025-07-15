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
#define DEG2RAD 0.0174532925199432957f
float deltaTime = 0.016f;
bool keyStates[256];
int id = 6;
Scene* scene = nullptr;
int Init ( ESContext *esContext )
{
	scene = new Scene();
	return scene->Init();

}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	scene->Render(id);
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

}

void Update ( ESContext *esContext, float deltaTime )
{

	if (keyStates['I'] )
	{
		Camera::GetInstance()->Rotateup(deltaTime);
	}
	if (keyStates['E'])
	{
		Camera::GetInstance()->MoveBackward(deltaTime);
	}
	if (keyStates['Q'])
	{
		Camera::GetInstance()->MoveForward(deltaTime);
	}
	if (keyStates['K'])
	{
		Camera::GetInstance()->Rotatedown(deltaTime);
	}
	if (keyStates['J'])
	{
		Camera::GetInstance()->RotateLeft(deltaTime);
	}
	if (keyStates['L'])
	{
		Camera::GetInstance()->RotateRight(deltaTime);
	}
	if (keyStates['W'])
	{
		Camera::GetInstance()->MoveUp(deltaTime);
	}
	if (keyStates['S'])
	{
		Camera::GetInstance()->MoveDown(deltaTime);
	}
	if (keyStates['A'])
	{
		Camera::GetInstance()->MoveLeft(deltaTime);
	}
	if (keyStates['D'])
	{
		Camera::GetInstance()->MoveRight(deltaTime);
	}
	if (keyStates['X'])
	{
		Camera::GetInstance()->testRotateY(deltaTime);
	}
	if (keyStates['C'])
	{
		Camera::GetInstance()->testRotateX(deltaTime);
	}
	if (keyStates['R'] && Camera::GetInstance()->fov <= 179)
	{
		Camera::GetInstance()->fov += 1;
		Camera::GetInstance()->UpdateProjMatrix(4/3);

	}
	if (keyStates['T'] && Camera::GetInstance()->fov >=1)
	{
		Camera::GetInstance()->fov -= 1;
		Camera::GetInstance()->UpdateProjMatrix(4 / 3);
	}
	if (keyStates['Z'])
	{
		Camera::GetInstance()->RotateAroundTarget(deltaTime*2);
		//Camera::GetInstance()->MoveLeft(deltaTime);
	}
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	keyStates[key] = bIsPressed;
	/*if (bIsPressed)
	{
		if ( key == 'S')
			Camera::GetInstance()->MoveDown(deltaTime);
		if (key == 'I' && key == 'L') {
			Camera::GetInstance()->Rotateup(deltaTime);
			Camera::GetInstance()->RotateLeft(deltaTime);
		}
		if ( key == 'w')
			Camera::GetInstance()->MoveUp(deltaTime);
		if (key == 'D')
			Camera::GetInstance()->MoveRight(deltaTime);
		if (key == 'A')
			Camera::GetInstance()->MoveLeft(deltaTime);
		if ( key == 'Q')
			Camera::GetInstance()->MoveForward(deltaTime);
		if (key == 'E')
			Camera::GetInstance()->MoveBackward(deltaTime);
		if (key == 'L')
			Camera::GetInstance()->RotateLeft(deltaTime);
		if (key == 'J')
			Camera::GetInstance()->RotateRight(deltaTime);
		if (key == 'I') {
			Camera::GetInstance()->Rotateup(deltaTime);
		}
		if ( key == 'K')
			Camera::GetInstance()->Rotatedown(deltaTime);
	}*/
}

void CleanUp()
{
	if (scene)
	{
		delete scene;
		scene = nullptr;
	}
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

