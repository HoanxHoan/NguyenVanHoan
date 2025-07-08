#pragma once
#include "Shaders.h"
#include <GLES3/gl3.h>
class Scene
{
public:
	Scene();
	~Scene();
	Matrix modelMatrix;
	Matrix viewMatrix;
	Matrix projMatrix;
};
