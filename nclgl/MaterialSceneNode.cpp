#include "MaterialSceneNode.h"
#include "MeshMaterial.h"
#include "AssetManager.h"
#include "ShaderManager.h"

MaterialSceneNode::MaterialSceneNode(const char* shader, const char* name, Vector3 position, Vector3 scale) : SceneNode(nullptr,	Vector4(1,1,1,1), shader)
{
	mesh = AssetManager::GetInstance().GetMesh(name);//("Gledista_Triacanthos_2.msh");
	
	material = AssetManager::GetInstance().GetMeshMaterial(name);

	Matrix4 localtransform;
	localtransform.SetPositionVector(position);
	transform = localtransform;
	modelScale = scale;
}

MaterialSceneNode::~MaterialSceneNode()
{
}

void MaterialSceneNode::Draw(OGLRenderer& r)
{
	Shader* activeShader = ShaderManager::GetInstance().GetShader(shader);
	PreDraw(r);
	
	glUniform1i(glGetUniformLocation(activeShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(activeShader->GetProgram(),
		"bumpTex"), 1);
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->matTextures[i]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material->matTextureNormals[i]);
		mesh->DrawSubMesh(i);
	}
}
