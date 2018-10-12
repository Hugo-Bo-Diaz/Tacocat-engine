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
	names.clear();

	const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{	
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			NOTmesh* m = new NOTmesh();

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
						App->UI->console->AddLog("geometry messed up");
					else
						memcpy(&m->index[i * 3], iterator->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}				
			if (iterator->HasTextureCoords(0))
			{
				//m->num_index = iterator->mNumFaces * 3;
				//m->index = new uint[m->num_index]; // assume each face is a triangle
				m->tex_coords = new float[m->num_index * 2];
				uint w = 0;
				for (uint i = 0; i < iterator->mNumVertices*2; i+=2)
				{
					memcpy(&m->tex_coords[i], &iterator->mTextureCoords[0][w].x, sizeof(float));
					memcpy(&m->tex_coords[i+1], &iterator->mTextureCoords[0][w].y, sizeof(float));
					++w;
				}
			}
			//if (iterator->HasNormals())
			//{
			//	m->normals = new float[iterator->mNumFaces * 3]; // assume each face is a triangle
			//	for (uint i = 0; i < iterator->mNumFaces * 3; i)
			//	{
			//		m->normals[i] = iterator->mNormals[i++].x;
			//		m->normals[i] = iterator->mNormals[i++].y;
			//		m->normals[i] = iterator->mNormals[i++].z;
			//	}
			//}
		
		glGenBuffers(1, (GLuint*) &(m->buffer_id));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->buffer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*m->num_index, &m->index[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		App->renderer3D->AddMesh(m);
		name = iterator->mName.C_Str();
		aiQuaterniont <float> quat;
		scene->mRootNode->mChildren[i]->mTransformation.Decompose(m->scaling,quat,m->position);
		m->rotation = quat.GetEuler();
		
		//float minx=0;
		//float miny=0;
		//float minz=0;
		//float maxx=0;
		//float maxy=0;
		//float maxz=0;
		m->bounding_box = m->bounding_box.MinimalEnclosingAABB((float3*)m->vertex,m->num_vertex);

		//int lmao = 0;
		}
		
		App->UI->console->AddLog("Loaded %d meshes %s", scene->mNumMeshes, file);

		if (scene->HasMaterials())
		{
			for (unsigned int i = 0; i < scene->mNumMaterials; i++)
			{
				const aiMaterial* material = scene->mMaterials[i];
				aiString texturePath;

				unsigned int numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);   // always 0

				if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
				{
					material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
					App->tex_loader->LoadTexture(texturePath.C_Str());
				}

			}

		}

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

	if (texture != 0)
	{
		glBindTexture(GL_TEXTURE_2D, texture);	
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glTexCoordPointer(2, GL_FLOAT, 0, &tex_coords[0]);

	glVertexPointer(3, GL_FLOAT, 0, &vertex[0]);
	glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);


	if (texture != 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);

	}	
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void NOTmesh::draw_bounding_box()
{
	glBegin(GL_LINES);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.minPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);


	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);
	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.minPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);

	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.minPoint.z);
	glVertex3f(bounding_box.maxPoint.x, bounding_box.maxPoint.y, bounding_box.maxPoint.z);


	glEnd();
}