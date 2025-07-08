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

	Object* obj;
	
};
