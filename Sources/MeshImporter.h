#pragma once
#include "Importer.h"
#include <vector>
#include "Globals.h"
#include "Primitive.h"

#include "MeshComponent.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"

#include "MathGeoLib/MathGeoLib.h"

#define MESH_PATH "/Meshes"

class Scene;

class MeshImporter : public Importer
{
public:

	MeshImporter() {};

	void CreateOwnFile(Component_Mesh* mesh_to_save);
	void Import();

	std::vector<std::string> filenames;

	void LoadCustomMeshFiles(Scene* scene_to);

};