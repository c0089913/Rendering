#include "Renderer.h"
#include "../nclgl/MaterialSceneNode.h"
#include "../nclgl/AnimatedSceneNode.h"
#include <algorithm>
#include "../nclgl/Light.h"
#include "../nclgl/ShaderManager.h"
#include "../nclgl/AssetManager.h"

#define PROJ_MATRIX Matrix4::Perspective(1.0f, 15000.0f,(float)width / (float)height, 45.0f);
#define NUM_LIGHTS 3
#define POST_PASSES 2
#define TERRAIN_POINT(x,z) terrain->heightMap->GetHeightAtPoint(z, x)
Renderer::Renderer(Window &parent) : OGLRenderer(parent)	
{
	if (!ShaderManager::GetInstance().InitSuccess())
		return;
	AssetManager::GetInstance();

	SetupScene();
	SetupCamera();
	SetupLights();
	SetupFrameBuffers();
	
	sphere = AssetManager::GetInstance().GetMesh("Sphere");
	quad = AssetManager::GetInstance().GetMesh("Quad");	

	projMatrix = PROJ_MATRIX;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	cubeMap = skybox.GetCubemap();

	postProcessing = false;
	init = true;
}

void Renderer::SetupLights()
{
	Vector3 dim = terrain->heightMap->GetHeightmapSize();
	lights = new Light[NUM_LIGHTS];

	Light* l = lights;
	l->SetPosition(Vector4(dim.x * 0.7f,1500.0f,dim.z * 0.7f, 1.0f));
	l->SetColour(Vector4(1, 1, 1, 1));
	l->SetRadius(7500.0f);
	l->SetIntensity(5.0f);

	l = &lights[1];
	l->SetPosition(Vector4(dim.x * 0.5, 0.0f, dim.z * 0.5f, 1.0f));
	l->SetColour(Vector4(0.74f, 0.22f, 0.62f, 1));
	l->SetRadius(2000.0f);
	l->SetIntensity(2.0f);

	l = &lights[2];
	l->SetPosition(Vector4(dim.x * 0.2f, 0.0f, dim.z * 0.5f, 1.0f));
	l->SetColour(Vector4(0.67f, 0.0f, 1.0f, 1));
	l->SetRadius(2000.0f);
	l->SetIntensity(2.0f);
}

void Renderer::SetupFrameBuffers()
{
	glGenFramebuffers(1, &bufferFBO);
	glGenFramebuffers(1, &lightFBO);
	glGenFramebuffers(1, &skyboxFBO);
	glGenFramebuffers(1, &postProcessFBO);
	glGenFramebuffers(1, &blurFBO);

	GLenum buffers[2] = {
	GL_COLOR_ATTACHMENT0 ,
	GL_COLOR_ATTACHMENT1
	};
	GenerateScreenTexture(bufferDepthTex, true);
	GenerateScreenTexture(bufferColourTex);
	GenerateScreenTexture(bufferNormalTex);
	GenerateScreenTexture(lightDiffuseTex);
	GenerateScreenTexture(lightSpecularTex);
	GenerateScreenTexture(skyboxTex);
	GenerateScreenTexture(postProcessTex[0]);
	GenerateScreenTexture(postProcessTex[1]);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, bufferNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE) {
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, lightDiffuseTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, lightSpecularTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, skyboxFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, skyboxTex, 0);
	glDrawBuffers(1, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, postProcessTex[0], 0);
	glDrawBuffers(1, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::SetupScene()
{
	terrain = new HeightMapNode(TEXTUREDIR"island noise (2).png", *this, "terrain");

	root = new SceneNode();
	SceneNode* node = new MaterialSceneNode("material", "Tree", terrain->heightMap->GetHeightAtPoint(512, 512), Vector3(15, 15, 15));
	root->AddChild(node);
	node = new MaterialSceneNode("material", "Tree", terrain->heightMap->GetHeightAtPoint(600, 600), Vector3(15, 15, 15));
	root->AddChild(node);
	node = new MaterialSceneNode("material", "Tree", terrain->heightMap->GetHeightAtPoint(354, 279), Vector3(15, 15, 15));
	root->AddChild(node);
	node = new MaterialSceneNode("material", "Tree", terrain->heightMap->GetHeightAtPoint(569, 609), Vector3(15, 15, 15));
	root->AddChild(node);
	node = new MaterialSceneNode("material", "Tree", terrain->heightMap->GetHeightAtPoint(275, 358), Vector3(15, 15, 15));
	root->AddChild(node);
	node = new MaterialSceneNode("material", "Tree", terrain->heightMap->GetHeightAtPoint(557, 621), Vector3(15, 15, 15));
	root->AddChild(node);
	node = new MaterialSceneNode("material", "Tree", terrain->heightMap->GetHeightAtPoint(267, 398), Vector3(15, 15, 15));
	root->AddChild(node);
	node = new MaterialSceneNode("material", "Tree", terrain->heightMap->GetHeightAtPoint(565, 684), Vector3(15, 15, 15));
	root->AddChild(node);
	node = new MaterialSceneNode("material", "Tree", terrain->heightMap->GetHeightAtPoint(315, 482), Vector3(15, 15, 15));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(400, 400), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(405, 405), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(410, 410), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(415, 415), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(400, 405), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(400, 410), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(400, 415), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(405, 400), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(405, 410), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(405, 415), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(351, 503), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(356, 508), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(351, 512), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(356, 512), Vector3(20, 20, 20));
	root->AddChild(node);
	node = new AnimatedSceneNode("animation", "soldier", "soldier", terrain->heightMap->GetHeightAtPoint(345, 502), Vector3(20, 20, 20));
	root->AddChild(node);

}

void Renderer::SetupCamera()
{
	float height = terrain->heightMap->GetHeightmapSize().y * 0.5f;
	camera = new Camera(0.0f, 0.0f, (Vector3(0, 100, 750.0f)));
	camera->SetPosition(Vector3(TERRAIN_POINT(616, 717).x, height, TERRAIN_POINT(616, 717).z));

	camera->AddControlPoint(Vector3(2329, height, 2825),
							Vector3(2357, height, 2625));
	camera->AddControlPoint(Vector3(2505, height, 2348),
							Vector3(2296, height, 2191));
	camera->AddControlPoint(Vector3(1844, height, 1530),
							Vector3(1692, height, 1638));
	camera->AddControlPoint(Vector3(1059, height, 1331),
		Vector3(1316, height, 1548));
	camera->AddControlPoint(Vector3(585, height, 1798),
		Vector3(883, height, 1703));
	camera->AddControlPoint(Vector3(457, height, 1953),
		Vector3(620, height, 1930));
	camera->AddControlPoint(Vector3(750, height, 2786),
		Vector3(798, height, 2475));
}

void Renderer::GenerateScreenTexture(GLuint& into, bool depth) {
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//GLuint format = depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8;
	GLuint format = depth ? GL_DEPTH_COMPONENT24 : GL_SRGB8_ALPHA8;
	GLuint type = depth ? GL_DEPTH_COMPONENT : GL_RGBA;


	glTexImage2D(GL_TEXTURE_2D, 0,
		format, width, height, 0, type, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Renderer::~Renderer(void)	{
	delete root;
	delete camera;
	delete terrain;
	delete[] lights;

	glDeleteTextures(1, &bufferColourTex);
	glDeleteTextures(1, &bufferNormalTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteTextures(1, &lightDiffuseTex);
	glDeleteTextures(1, &lightSpecularTex);
	glDeleteTextures(1, &postProcessTex[0]);
	glDeleteTextures(1, &postProcessTex[1]);

	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &lightFBO);
	glDeleteFramebuffers(1, &postProcessFBO);
	glDeleteFramebuffers(1, &blurFBO);
}

void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	root->Update(dt);
	waterNode.Update(dt);
}

void Renderer::RenderScene()	{

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);		
	glDisable(GL_FRAMEBUFFER_SRGB);//lighting in linear space
	FillBuffers();
	DrawLights();
	skybox.Draw(*this);
	glEnable(GL_FRAMEBUFFER_SRGB);//after lighting is done gamma correction enabled
	if (postProcessing)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	}
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	CombineBuffers();
	if (postProcessing)
	{		
		PostProcess();		
		FinalScene();
	}
}

void Renderer::BuildNodeLists(SceneNode* from) {

		Vector3 dir = from->GetWorldTransform().GetPositionVector() -
			camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
		}
		else {
			nodeList.push_back(from);
		}


	for (vector <SceneNode*>::const_iterator i =
		from->GetChildIteratorStart();
		i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));
	}
}
void Renderer::SortNodeLists() {
	std::sort(transparentNodeList.rbegin(), //note the r!
		transparentNodeList.rend(), //note the r!
		SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(),
		nodeList.end(),
		SceneNode::CompareByCameraDistance);
}
void Renderer::DrawNodes() {
	for (const auto& i : nodeList) {
		DrawNode(i);
	}
	for (const auto& i : transparentNodeList) {
		DrawNode(i);
	}
}
void Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {
		n->Draw(*this);
	}
}

void Renderer::FillBuffers()
{
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	projMatrix = PROJ_MATRIX;
	viewMatrix = camera->BuildViewMatrix();

	BuildNodeLists(root);
    SortNodeLists();	
	DrawNodes();	
	ClearNodeLists();
	terrain->Draw(*this);
	waterNode.Draw(*this, camera, terrain->heightMap);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawLights()
{
	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	Shader* activeShader = ShaderManager::GetInstance().GetShader("light");
	BindShader(activeShader);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_ONE, GL_ONE);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);

	glUniform1i(glGetUniformLocation(
		activeShader->GetProgram(), "depthTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);

	glUniform1i(glGetUniformLocation(
		activeShader->GetProgram(), "normTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bufferNormalTex);

	glUniform3fv(glGetUniformLocation(activeShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform2f(glGetUniformLocation(activeShader->GetProgram(),
		"pixelSize"), 1.0f / width, 1.0f / height);

	Matrix4 modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(
		activeShader->GetProgram(), "modelMatrix"),
		1, false, modelMatrix.values);

	Matrix4 invViewProj = (projMatrix * viewMatrix).Inverse();
	glUniformMatrix4fv(glGetUniformLocation(
		activeShader->GetProgram(), "inverseProjView"),
		1, false, invViewProj.values);
	UpdateShaderMatrices();
	for (int i = 0; i < NUM_LIGHTS; ++i) {
		Light& l = lights[i];
		SetShaderLight(l);
		sphere->Draw();
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);

	glDepthMask(GL_TRUE);

	glClearColor(0.2f, 0.2f, 0.2f, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::CombineBuffers()
{
	Shader* combineShader = ShaderManager::GetInstance().GetShader("combine");
	BindShader(combineShader);
	Matrix4 modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(
		combineShader->GetProgram(), "modelMatrix"),
		1, false, modelMatrix.values);
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex);

	glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "diffuseLight"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, lightDiffuseTex);

	glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "specularLight"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, lightSpecularTex);

	glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "skyboxTex"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, skyboxTex);

	glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "depthTex"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);

	quad->Draw();
}

void Renderer::PostProcess()
{
	glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, postProcessTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	Shader* ppShader = ShaderManager::GetInstance().GetShader("postprocess");
	BindShader(ppShader);
	Matrix4 modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(
		ppShader->GetProgram(), "modelMatrix"),
		1, false, modelMatrix.values);
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(
		ppShader->GetProgram(), "sceneTex"), 0);
	for (int i = 0; i < POST_PASSES; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, postProcessTex[1], 0);
		glUniform1i(glGetUniformLocation(ppShader->GetProgram(),
			"isVertical"), 0);

		glBindTexture(GL_TEXTURE_2D, postProcessTex[0]);
		quad->Draw();

		glUniform1i(glGetUniformLocation(ppShader->GetProgram(),"isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, postProcessTex[0], 0);
		glBindTexture(GL_TEXTURE_2D, postProcessTex[1]);
		quad->Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::FinalScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	Shader* sceneShader = ShaderManager::GetInstance().GetShader("scene");
	BindShader(sceneShader);
	Matrix4 modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(
		sceneShader->GetProgram(), "modelMatrix"),
		1, false, modelMatrix.values);
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, postProcessTex[0]);
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);
	quad->Draw();
}
