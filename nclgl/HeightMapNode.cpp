#include "HeightMapNode.h"
#include "Light.h"
#include "Camera.h"
#include "ShaderManager.h"

void HeightMapNode::Draw(OGLRenderer& r)
{
	Shader* activeShader = ShaderManager::GetInstance().GetShader(shader);
	PreDraw(r);
	Matrix4 textureMatrix;
	//textureMatrix = Matrix4::Scale(Vector3(100, 100, 100));
	textureMatrix =
		Matrix4::Scale(Vector3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(glGetUniformLocation(activeShader->GetProgram(), "textureMatrix"), 1, false, textureMatrix.values);

	glUniform1i(glGetUniformLocation(activeShader->GetProgram(),
		"diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(glGetUniformLocation(activeShader->GetProgram(),
		"stoneTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, stoneTex);

	glUniform1i(glGetUniformLocation(activeShader->GetProgram(),
		"slopeTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, slopeTex);

	glUniform1i(glGetUniformLocation(activeShader->GetProgram(),
		"splatTex"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, splatTex);

	glUniform1i(glGetUniformLocation(activeShader->GetProgram(),
		"bumpTex"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, bumpTex);

	glUniform2f(glGetUniformLocation(activeShader->GetProgram(),
		"resolution"), heightMap->GetHeightmapSize().x, heightMap->GetHeightmapSize().z);

	glUniform3fv(glGetUniformLocation(activeShader->GetProgram(),
		"cameraPos"), 1, (float*)&r.camera->GetPosition());
	heightMap->Draw();
}

HeightMapNode::HeightMapNode(const std::string& name, OGLRenderer &r, const char*shader) 
	: SceneNode(nullptr, Vector4(1,1,1,1),shader)
{
	heightMap = new HeightMap(name);
	texture = SOIL_load_OGL_texture(TEXTUREDIR"forest.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (!texture) {
		return;
	}

	r.SetTextureRepeating(texture, true);


	stoneTex = SOIL_load_OGL_texture(TEXTUREDIR"rocks.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (!stoneTex) {
		return;
	}

	r.SetTextureRepeating(stoneTex, true);

	slopeTex = SOIL_load_OGL_texture(TEXTUREDIR"slope.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (!slopeTex) {
		return;
	}

	r.SetTextureRepeating(slopeTex, true);
	splatTex = SOIL_load_OGL_texture(TEXTUREDIR"island splatmap.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);


	bumpTex = SOIL_load_OGL_texture(TEXTUREDIR"island normal.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (!bumpTex) {
		return;
	}

	r.SetTextureRepeating(bumpTex, true);
}
