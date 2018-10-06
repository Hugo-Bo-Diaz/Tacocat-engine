#include "Globals.h"
#include "Application.h"
#include "ModuleMeshLoader.h"
#include "parson\parson.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

#pragma comment(lib,"Assimp/libx86/assimp.lib")


ModuleMeshLoader::ModuleMeshLoader(bool start_enabled) : Module(start_enabled)
{
	name = "Mesh Loader";
}

ModuleMeshLoader::~ModuleMeshLoader()
{}

// Load assets
bool ModuleMeshLoader::Start()
{
	bool ret = true;

	//Load("warrior.FBX");

	return ret;
}

// Load assets
bool ModuleMeshLoader::CleanUp()
{
	App->UI->console->AddLog("Unloading Intro scene");

	return true;
}

uint* ModuleMeshLoader::Load(const char* file)
{
	const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{	
		NOTmesh* m = new NOTmesh();
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* iterator = scene->mMeshes[i];

			m->num_vertex = iterator->mNumVertices;
			m->vertex = new float[m->num_vertex * 3];
			memcpy(m->vertex, iterator->mVertices, sizeof(float) * m->num_vertex * 3);
			if (iterator->HasFaces())
			{
				m->num_index = iterator->mNumFaces * 3;
				m->index = new uint[m->num_index]; // assume each face is a triangle
				for (uint i = 0; i < iterator->mNumFaces; ++i)
				{
					if (iterator->mFaces[i].mNumIndices != 3)
						//LOG("WARNING, geometry face with != 3 indices!");
						App->UI->console->AddLog("geometry messed up");
					else
						memcpy(&m->index[i * 3], iterator->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}
		glGenBuffers(1, (GLuint*) &(m->buffer_id));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->buffer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*m->num_index, &m->index[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		}
		App->renderer3D->AddElement(m);

		aiReleaseImport(scene);
	}
	else
	{
		App->UI->console->AddLog("file not found");
	}
	
	return 0;
}
void NOTmesh::draw()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
	glVertexPointer(3, GL_FLOAT, 0, &vertex[0]);
	glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}