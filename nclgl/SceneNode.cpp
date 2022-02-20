#include "SceneNode.h"
#include "ShaderManager.h"

SceneNode::SceneNode(Mesh* mesh, Vector4 colour, const char* program) {
	this->mesh = mesh;
	this->colour = colour;
	parent = NULL;
	modelScale = Vector3(1, 1, 1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
	texture = 0;
	shader = program;
}

SceneNode ::~SceneNode(void) {
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* s) {
	children.push_back(s);
	s->parent = this;
}
void SceneNode::Draw(OGLRenderer& r) {

	if (mesh) { mesh->Draw(); }
}
void SceneNode::PreDraw(OGLRenderer& r)
{
	if (shader)
	{
		r.BindShader(ShaderManager::GetInstance().GetShader(shader));
	}
	r.UpdateShaderMatrices();
	Matrix4 model = GetWorldTransform() * Matrix4::Scale(GetModelScale());
	glUniformMatrix4fv(glGetUniformLocation(r.currentShader->GetProgram(), "modelMatrix")   ,	1,false, model.values);
}
void SceneNode::Update(float dt) {
	if (parent) { //This node has a parent ...
		worldTransform = parent->worldTransform * transform;
	}
	else { //Root node , world transform is local transform!
		worldTransform = transform;
	}
	for (vector <SceneNode*>::iterator i = children.begin();
		i != children.end(); ++i) {
		(*i)->Update(dt);
	}
}