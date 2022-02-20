#include "WaterNode.h"
#include "ShaderManager.h"
#include "AssetManager.h"

WaterNode::WaterNode()
{
	texture = SOIL_load_OGL_texture(TEXTUREDIR"water1.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	dudvTex = SOIL_load_OGL_texture(TEXTUREDIR"dudv.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	bumpTex = SOIL_load_OGL_texture(TEXTUREDIR"waterbump.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (!texture || !dudvTex || !bumpTex)
		return;
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, dudvTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, bumpTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	shader = "water";
	mesh = AssetManager::GetInstance().GetMesh("Quad");
	waterRotate = 0.0f;
	waterCycle = 0.0f;
	distortionSpeed = 0.0f;
}

void WaterNode::Update(float dt)
{
	SceneNode::Update(dt);	
	
	waterCycle += dt * 0.02f;
	waterRotate += dt * 0.02f;
	distortionSpeed += dt * 0.2f;
	if (distortionSpeed > 1)
		distortionSpeed -= 1;
}

void WaterNode::Draw(OGLRenderer& r, Camera *camera, HeightMap *heightMap)
{
	Shader* activeShader = ShaderManager::GetInstance().GetShader(shader);
	glDisable(GL_CULL_FACE);
	PreDraw(r);
	glUniform3fv(glGetUniformLocation(activeShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(activeShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(activeShader->GetProgram(), "dudvTex"), 1);
	glUniform1i(glGetUniformLocation(activeShader->GetProgram(), "cubeTex"), 2);
	glUniform1i(glGetUniformLocation(activeShader->GetProgram(), "bumpTex"), 3);
	glUniform1f(glGetUniformLocation(activeShader->GetProgram(), "speed"), waterCycle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dudvTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, r.cubeMap);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, bumpTex);
	Vector3 hSize = heightMap->GetHeightmapSize();

	Vector3 translate = hSize * 0.5;
	translate.y /= 2;
	transform.ToIdentity();
	transform =
		Matrix4::Translation(translate) *
		Matrix4::Scale(hSize *20.0f) *
		Matrix4::Rotation(90, Vector3(1, 0, 0));

	Matrix4 textureMatrix;
	//textureMatrix = Matrix4::Scale(Vector3(100, 100, 100));
	textureMatrix =
		Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) *
		Matrix4::Scale(Vector3(100, 100, 100)) *
		Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));

	//PreDraw(r);
	glUniformMatrix4fv(glGetUniformLocation(activeShader->GetProgram(), "textureMatrix"), 1, false, textureMatrix.values);
	mesh->Draw();
	glEnable(GL_CULL_FACE);
}

