#pragma once
#include "Importer.h"
#include <vector>
#include "Globals.h"
#include "Primitive.h"

#include "Material.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"

#include "MathGeoLib/MathGeoLib.h"

#define MESH_PATH "/Meshes"

class Scene;

class TextureImporter : public Importer
{
public:

	TextureImporter() {};

	void CreateOwnFile(Material* material_to_save);
	void Import(uint mesh_uid);

	std::vector<std::string> filenames;

	void LoadCustomMeshFiles(Scene* scene_to);

};