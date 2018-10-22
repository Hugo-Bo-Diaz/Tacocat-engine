#pragma once
#include "Importer.h"
#include <array>
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

	void CreateOwnFile(const char* path_to_file, const char* new_name);

};