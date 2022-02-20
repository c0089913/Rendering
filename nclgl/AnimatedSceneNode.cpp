#include "AnimatedSceneNode.h"
#include "AssetManager.h"
#include "MeshAnimation.h"
#include "ShaderManager.h"
#include "MeshMaterial.h"

AnimatedSceneNode::AnimatedSceneNode(const char* shader, const char* name, const char* animationName, Vector3 position, Vector3 scale):
	MaterialSceneNode(shader, name, position, scale)
{
	animation = AssetManager::GetInstance().GetAnimation(animationName);
	currentFrame = 0;
	frameTime = 0.0f;
}

void AnimatedSceneNode::Draw(OGLRenderer& r)
{
	Shader* activeShader = ShaderManager::GetInstance().GetShader(shader);
	PreDraw(r);

	glUniform1i(glGetUniformLocation(activeShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(activeShader->GetProgram(),
		"bumpTex"), 1);
	vector <Matrix4 > frameMatrices;

	const Matrix4* invBindPose = mesh->GetInverseBindPose();
	const Matrix4* frameData = animation->GetJointData(currentFrame);

	for (unsigned int i = 0; i < mesh->GetJointCount(); ++i) {
		frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
	}

	int j = glGetUniformLocation(activeShader->GetProgram(), "joints");
	glUniformMatrix4fv(j, frameMatrices.size(), false,
		(float*)frameMatrices.data());
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->matTextures[i]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material->matTextureNormals[i]);
		mesh->DrawSubMesh(i);
	}
}

void AnimatedSceneNode::Update(float dt)
{
	SceneNode::Update(dt);
	frameTime -= dt;
	while (frameTime < 0.0f) {
		currentFrame = (currentFrame + 1) % animation->GetFrameCount();
		frameTime += 1.0f / animation->GetFrameRate();
	}
}
