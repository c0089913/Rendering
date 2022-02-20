#pragma once
#include "SceneNode.h"
#include "OGLRenderer.h"
#include "HeightMap.h"

class HeightMapNode : public SceneNode
{
public:
	HeightMap* heightMap;
	GLuint stoneTex;
	GLuint slopeTex;
	GLuint splatTex;
	GLuint bumpTex;
	virtual void Draw(OGLRenderer &r);
	HeightMapNode(const std::string& name, OGLRenderer &r, const char *shader);
	~HeightMapNode() { delete heightMap; }
};