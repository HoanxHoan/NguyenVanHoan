#pragma once
#include "../Shaders.h"
#include "../GameObject/Object.h"
#include "../GameObject/Textures.h"
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
