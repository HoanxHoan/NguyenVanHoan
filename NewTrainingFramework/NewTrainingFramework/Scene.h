#pragma once
#include "Shaders.h"
#include "Object.h"
#include <GLES3/gl3.h>
class Scene
{
public:
	Scene();
	~Scene();
	Matrix modelMatrix;
	Matrix viewMatrix;
	Matrix projMatrix;
	bool Init(ESContext* esContext);
	void Update(ESContext* esContext, float deltaTime);
	void Draw(ESContext* esContext);
private:
	void getViewMatrix(float out[4][4], Vector3 eye, Vector3 at, Vector3 up);

	Object obj;
	Shaders myShaders;
};
