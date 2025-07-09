#pragma once
#include "Shaders.h"
#include "Object.h"
#include "Textures.h"
#include "Shaders.h"
#include <GLES3/gl3.h>
#include "ResourceManager.h"
class Scene
{
public:
	Scene();
	~Scene();

	bool Init(ESContext* esContext);
	void Update(ESContext* esContext, float deltaTime);
	void Draw(ESContext* esContext);

	Object* obj;
	
};
