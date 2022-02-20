#include "SkyBoxNode.h"
#include "ShaderManager.h"

SkyBoxNode::SkyBoxNode()
{
	mesh = Mesh::GenerateQuad();
	shader = "skybox";
	texture = SOIL_load_OGL_cubemap(
	/*	TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
		TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg",
		TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg",*/
		TEXTUREDIR"corona_rt.png", TEXTUREDIR"corona_lf.png",
		TEXTUREDIR"corona_up.png", TEXTUREDIR"corona_dn.png",
		TEXTUREDIR"corona_bk.png", TEXTUREDIR"corona_ft.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

}

void SkyBoxNode::Draw(OGLRenderer& r)
{	
	glBindFramebuffer(GL_FRAMEBUFFER, r.skyboxFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	Shader* activeShader = ShaderManager::GetInstance().GetShader(shader);	
	r.BindShader(activeShader);
	r.UpdateShaderMatrices();
	
	glUniform1i(glGetUniformLocation(activeShader->GetProgram(),
		"cubeTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);	
	mesh->Draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
