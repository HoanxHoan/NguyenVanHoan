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
#define DEG2RAD 0.0174532925199432957f
GLuint vboId;
GLuint iboId;
GLuint textureID;
Shaders myShaders;
int iWidth, bpp, iHeight;
void LookAt(float out[4][4], Vector3 eye, Vector3 at, Vector3 up)
{
	Vector3 zaxis = (at - eye).Normalize();
	Vector3 xaxis = up.Cross(zaxis).Normalize();
	Vector3 yaxis = zaxis.Cross(xaxis);

	// Column-major order (OpenGL)
	out[0][0] = xaxis.x;
	out[0][1] = yaxis.x;
	out[0][2] = zaxis.x;
	out[0][3] = 0;

	out[1][0] = xaxis.y;
	out[1][1] = yaxis.y;
	out[1][2] = zaxis.y;
	out[1][3] = 0;

	out[2][0] = xaxis.z;
	out[2][1] = yaxis.z;
	out[2][2] = zaxis.z;
	out[2][3] = 0;

	out[3][0] = -xaxis.Dot(eye);
	out[3][1] = -yaxis.Dot(eye);
	out[3][2] = -zaxis.Dot(eye);
	out[3][3] = 1;
}

void LoadNFG(const char* filename, Vertex* verticesData, unsigned int* indices)
{
	FILE* f;

	if (fopen_s(&f, filename, "rb") != 0)
	{
		std::cout << "Cannot open file.\n";
		return;
	}

	char temp[256];
	int nrVertices = 0;
	fscanf_s(f, "%s %d", temp, sizeof(temp), &nrVertices); // NrVertices: <num>

	
	for (int i = 0; i < nrVertices; i++)
	{
		int index;
		Vertex v;

		fscanf_s(f, "%d.", &index); 

		
		fscanf_s(f, " pos:[%f, %f, %f];", &v.pos.x, &v.pos.y, &v.pos.z);
		
		fscanf_s(f, " norm:[%f, %f, %f];", &v.normal.x, &v.normal.y, &v.normal.z);
		
		fscanf_s(f, " binorm:[%f, %f, %f];", &v.binormal.x, &v.binormal.y, &v.binormal.z);
		
		fscanf_s(f, " tgt:[%f, %f, %f];", &v.tangent.x, &v.tangent.y, &v.tangent.z);
		
		fscanf_s(f, " uv:[%f, %f];", &v.uv.x, &v.uv.y);

		verticesData[i] = v;
	}

	int nrIndices = 0;
	fscanf_s(f, "%s %d", temp, sizeof(temp), &nrIndices); // NrIndices: <num>

	
	for (int i = 0; i < nrIndices / 3; i++)
	{
		int idxLine;
		unsigned int a, b, c;

		fscanf_s(f, "%d.", &idxLine);
		fscanf_s(f, " %d, %d, %d", &a, &b, &c);

		indices[i * 3 + 0] = a;
		indices[i * 3 + 1] = b;
		indices[i * 3 + 2] = c;
	}

	fclose(f);

	std::cout << "Loaded " << nrVertices << " vertices and " << nrIndices << " indices.\n";
}

int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	//triangle data (heap)
	Vertex* verticesData= new Vertex[512];
	unsigned int* indices= new unsigned int[2154];

	LoadNFG("../Resources/Models/Woman1.nfg", verticesData, indices);


	


	/*verticesData[0].pos.x =  0.0f;  verticesData[0].pos.y =  0.5f;  verticesData[0].pos.z =  0.0f;
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z =  0.0f;
	verticesData[2].pos.x =  0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z =  0.0f;*/

	//verticesData[0].color.x = 1.0f;  verticesData[0].color.y = 0.0f;  verticesData[0].color.z = 0.0f;
	//verticesData[1].color.x = 0.0f;  verticesData[1].color.y = 1.0f;  verticesData[1].color.z = 0.0f;
	//verticesData[2].color.x = 0.0f;  verticesData[2].color.y = 0.0f;  verticesData[2].color.z = 0.1f;


	/*verticesData[0].uv.x = 1.0f;  verticesData[0].uv.y = 0.0f; 
	verticesData[1].uv.x = 0.0f;  verticesData[1].uv.y = 1.0f; 
	verticesData[2].uv.x = 1.0f;  verticesData[2].uv.y = 1.0f; */
	


	//unsigned int indices[] = { 0,1,2 };

	//buffer object

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, 512 * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2154 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);


	char* imageData = LoadTGA("../Resources/Textures/Woman1.tga", &iWidth, &iHeight, &bpp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0,GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	delete[] verticesData;
	delete[] indices;
	//creation of shaders and program 
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaders.program);


	glBindTexture(GL_TEXTURE_2D, textureID);
	int iTextureLoc = glGetUniformLocation(myShaders.program, "u_texture");
	glUniform1i(iTextureLoc, 0);

	Matrix modelMatrix;
	modelMatrix.SetIdentity();

	Matrix scaleMatrix;
	scaleMatrix.SetScale(0.5f, 0.5f, 0.5f);

	Matrix rotationMatrix;
	rotationMatrix.SetRotationY(45.0f * DEG2RAD);

	Matrix translationMatrix;
	translationMatrix.SetTranslation(0.0f, 1.0f, 7.0f);
	modelMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	Matrix viewMatrix;
	Vector3 eye(0.0f, 1.0f, 3.0f);
	Vector3 at(0.0f, 1.0f, 0.0f);
	Vector3 up(0.0f, 1.0f, 0.0f);
	LookAt(viewMatrix.m, eye, at, up);


	Matrix projMatrix;
	projMatrix.SetPerspective(45.0f * DEG2RAD, 4 / 3, 0.1f, 100.0f);
	Matrix mvpMatrix = modelMatrix * viewMatrix * projMatrix;
	GLuint mvpLoc = glGetUniformLocation(myShaders.program, "u_mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (float*)mvpMatrix.m);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	// take location value from vertex/fragment shader. With Position we have location = 0
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)0 + sizeof(Vector3));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)12 + sizeof(Vector3));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)24 + sizeof(Vector3));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)36 + sizeof(Vector3));
	}


	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 2154, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{

}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{

}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Woman1", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

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

