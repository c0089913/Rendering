#include "AssetManager.h"
#include "Mesh.h"
#include "MeshMaterial.h"
#include "MeshAnimation.h"

AssetManager::AssetManager()
{
	Mesh* mesh;
	MeshMaterial* material;
	m_Meshes.insert({"Sphere", Mesh::LoadFromMeshFile("Sphere.msh")});
	m_Meshes.insert({ "Quad", Mesh::GenerateQuad() });

	mesh = Mesh::LoadFromMeshFile("Tree.msh");
	material = new MeshMaterial("Tree.mat"); material->PopulateTextures(mesh);
	m_Meshes.insert({"Tree", mesh});
	m_Materials.insert({"Tree", material });

	mesh = Mesh::LoadFromMeshFile("Role_T.msh");
	material = new MeshMaterial("Role_T.mat"); material->PopulateTextures(mesh);
	m_Meshes.insert({ "soldier", mesh });
	m_Materials.insert({ "soldier", material });
	m_Animations.insert({"soldier", new MeshAnimation("Role_T.anm")});
}
AssetManager& AssetManager::GetInstance()
{
	static AssetManager m_Instance;
	return m_Instance;
}

AssetManager::~AssetManager()
{
	for (auto& i : m_Materials)
	{
		delete i.second;
	}
	for (auto& i : m_Meshes)
	{
		delete i.second;
	}
	for (auto& i : m_Animations)
	{
		delete i.second;
	}
}

Mesh* AssetManager::GetMesh(const char* name)
{
	return m_Meshes.at(name);
}

MeshMaterial* AssetManager::GetMeshMaterial(const char*name)
{
	return m_Materials.at(name);
}

MeshAnimation* AssetManager::GetAnimation(const char*name)
{
	return m_Animations.at(name);
}
