#pragma once
#include "SceneNode.h"
class MeshMaterial;

class MaterialSceneNode :
	public SceneNode
{
protected:
	MeshMaterial* material;
public:
	MaterialSceneNode(const char *shader, const char *name, Vector3 position, Vector3 scale = Vector3(1,1,1));
	~MaterialSceneNode();
	void Draw(OGLRenderer& r);
};

