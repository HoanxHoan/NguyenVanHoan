#pragma once
#include "Math.h"
#include "Shaders.h"
struct Vertex
{
	Vector3 pos;
	Vector3 normal;
	Vector3 binormal;
	Vector3 tangent;
	Vector2 uv;
};