#pragma once
#include "../NCLGL/OGLRenderer.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/SkyBoxNode.h"
#include "../nclgl/HeightMapNode.h"
#include "../nclgl/Frustum.h"
#include "../nclgl/Camera.h"
#include "../nclgl/WaterNode.h"

class Light;
class Renderer : public OGLRenderer	{
public:
	Renderer(Window &parent);
	 ~Renderer(void);
	 void RenderScene()				override;
	 void UpdateScene(float msec)	override;
	 bool postProcessing;
protected:
	HeightMapNode* terrain;
	SkyBoxNode skybox;
	WaterNode waterNode;
	SceneNode *root;
	

	Mesh* sphere;
	Mesh* quad;

	void SetupLights();
	void SetupFrameBuffers();
	void SetupScene();
	void SetupCamera();
	void GenerateScreenTexture(GLuint& into, bool depth = false);
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists() {
		transparentNodeList.clear();
		nodeList.clear();
	}
	void DrawNodes();
	void DrawNode(SceneNode* n);
	void FillBuffers();
	void DrawLights();
	void CombineBuffers();
	void PostProcess();
	void FinalScene();
	vector <SceneNode*> transparentNodeList;
	vector <SceneNode*> nodeList;
};
