#pragma once
#include <map>
class Mesh;
class MeshMaterial;
class MeshAnimation;
class AssetManager
{
	std::map<const char*, Mesh*> m_Meshes;
	std::map<const char*, MeshMaterial*>m_Materials;
	std::map<const char*, MeshAnimation*>m_Animations;
	AssetManager();
public:
	static AssetManager& GetInstance();
	~AssetManager();
	Mesh* GetMesh(const char*);
	MeshMaterial* GetMeshMaterial(const char*);
	MeshAnimation* GetAnimation(const char*);
};

