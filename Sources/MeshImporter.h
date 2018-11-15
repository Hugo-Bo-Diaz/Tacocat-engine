#pragma once
#include "Importer.h"
#include <vector>
#include "Globals.h"
#include "Primitive.h"

#include "Mesh.h"

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

	void CreateOwnFile(Mesh* mesh_to_save);
	void Import(uint mesh_uid);

	std::vector<std::string> filenames;

	void LoadCustomMeshFiles(Scene* scene_to);

};