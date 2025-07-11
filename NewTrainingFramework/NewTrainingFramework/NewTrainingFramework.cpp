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


int Init ( ESContext *esContext )
{
	return Scene::GetInstance()->Init();

}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::GetInstance()->Render(0);
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

}

void Update ( ESContext *esContext, float deltaTime )
{

}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if (bIsPressed)
	{
		if (key == 's' || key == 'S')
			Camera::GetInstance()->MoveDown(0.016f);
		if (key == 'W' || key == 'w')
			Camera::GetInstance()->MoveUp(0.016f);
		if (key == 'd' || key == 'D')
			Camera::GetInstance()->MoveRight(0.016f);
		if (key == 'a' || key == 'A')
			Camera::GetInstance()->MoveLeft(0.016f);
		if (key == 'q' || key == 'Q')
			Camera::GetInstance()->MoveForward(0.016f);
		if (key == 'e' || key == 'E')
			Camera::GetInstance()->MoveBackward(0.016f);
	}
}

void CleanUp()
{
	Scene::Destroy();
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

