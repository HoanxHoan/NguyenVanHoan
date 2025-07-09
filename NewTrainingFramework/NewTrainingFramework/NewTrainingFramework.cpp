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
#include "Textures.h"
#include "Scene.h"
#define DEG2RAD 0.0174532925199432957f

Scene scene;


int Init ( ESContext *esContext )
{
	return scene.Init(esContext);

}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT);
	scene.Draw(esContext);

}

void Update ( ESContext *esContext, float deltaTime )
{

}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{

}

//void CleanUp()
//{
//	glDeleteBuffers(1, &vboId);
//}

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
	//CleanUp();

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}

