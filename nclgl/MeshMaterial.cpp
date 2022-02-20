#include "MeshMaterial.h"
#include <fstream>
#include <iostream>
#include "Mesh.h"
#include "common.h"

using std::ifstream;

MeshMaterial::MeshMaterial(const std::string& filename) {
	ifstream file(MESHDIR+filename);

	string dataType;
	file >> dataType;

	if (dataType != "MeshMat") {
		std::cout << "File " << filename << " is not a MeshMaterial!\n";
		return;
	}
	int version;
	file >> version;

	if (version != 1) {
		std::cout << "File " << filename << " has incompatible version " << version << "!\n";
		return;
	}

	int matCount;
	int meshCount;
	file >> matCount;
	file >> meshCount;

	materialLayers.resize(matCount);

	for (int i = 0; i < matCount; ++i) {
		string name;
		int count;
		file >> name;
		file >> count;
		file.ignore(256, '\n');
		for (int j = 0; j < count; ++j) {
			string entryData;
			std::getline(file, entryData);// file >> entryData;
			string channel;
			string file;
			size_t split = entryData.find_first_of(':');
			channel = entryData.substr(0, split);
			file = entryData.substr(split + 1);

			materialLayers[i].entries.insert(std::make_pair(channel, file));
		}
	}

	for (int i = 0; i < meshCount; ++i) {
		int entry;
		file >> entry;
		meshLayers.emplace_back(&materialLayers[entry]);
	}
}

const MeshMaterialEntry* MeshMaterial::GetMaterialForLayer(int i) const {
	if (i < 0 || i >= (int)meshLayers.size()) {
		return nullptr;
	}
	return meshLayers[i];
}

void MeshMaterial::PopulateTextures(Mesh* mesh)
{
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		const MeshMaterialEntry* matEntry = GetMaterialForLayer(i);

		const string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		matTextures.emplace_back(texID);
		int extensionIndex = filename->find_last_of('.');
		string name = filename->substr(0, extensionIndex);
		path = TEXTUREDIR + name + "normal" + filename->substr(extensionIndex);
		texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		matTextureNormals.emplace_back(texID);

	}
}
