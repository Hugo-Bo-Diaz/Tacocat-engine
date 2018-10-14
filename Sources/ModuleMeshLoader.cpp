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

void aiLog(const char* str, char* user)
{
	App->UI->console->AddLog(str);
}

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

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = aiLog;
	aiAttachLogStream(&stream);

	return ret;
}

// Load assets
bool ModuleMeshLoader::CleanUp()
{
	App->UI->console->AddLog("Unloading Intro scene");
	//aiDetachAllLogStreams();

	return true;
}

void ModuleMeshLoader::Load(const char* file)
{
	//PROVISIONAL
	for (std::vector<NOTmesh*>::iterator it = App->renderer3D->mesh_vector.begin(); it != App->renderer3D->mesh_vector.end(); it++)
	{
		//(*it)->~NOTmesh();
		delete (*it);
		(*it) = nullptr;
	}
	App->renderer3D->mesh_vector.clear();
	
	if (total_scene_bounding_box != nullptr)
	{
		delete total_scene_bounding_box;
		total_scene_bounding_box = nullptr;
	}

	total_scene_bounding_box = new AABB({100,100,100}, {-100,-100,-100});

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
				for (uint i = 0; i < iterator->mNumVertices * 2; i += 2)
				{
					memcpy(&m->tex_coords[i], &iterator->mTextureCoords[0][w].x, sizeof(float));
					memcpy(&m->tex_coords[i + 1], &iterator->mTextureCoords[0][w].y, sizeof(float));
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

			m->bounding_box = m->bounding_box.MinimalEnclosingAABB((float3*)m->vertex, m->num_vertex);

			//we want to move the model so that the center of this box is on the 0, 1/2height, 0
			if (scene->mNumMeshes < 2)//if i moved more than 1 the scene would not be in its place
			{
				float to_move_x, to_move_y, to_move_z;

				to_move_x = (m->bounding_box.maxPoint.x - m->bounding_box.minPoint.x) / 2 - m->bounding_box.maxPoint.x;//width/2 - max_x
				to_move_z = (m->bounding_box.maxPoint.z - m->bounding_box.minPoint.z) / 2 - m->bounding_box.maxPoint.z;//width/2 - max_z

				to_move_y = (m->bounding_box.maxPoint.y - m->bounding_box.minPoint.y) - m->bounding_box.maxPoint.y;


				m->bounding_box.minPoint += {to_move_x, to_move_y, to_move_z};
				m->bounding_box.maxPoint += {to_move_x, to_move_y, to_move_z};
				m->Move(to_move_x, to_move_y, to_move_z);
			}
			name = iterator->mName.C_Str();
			aiQuaterniont <float> quat;
			scene->mRootNode->mChildren[i]->mTransformation.Decompose(m->scaling, quat, m->position);
			m->rotation = quat.GetEuler();

			glGenBuffers(1, (GLuint*) &(m->buffer_id));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->buffer_id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*m->num_index, &m->index[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			App->renderer3D->AddMesh(m);
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
		for (std::vector<NOTmesh*>::iterator it = App->renderer3D->mesh_vector.begin(); it != App->renderer3D->mesh_vector.end(); it++)
		{
			if ((*it)->bounding_box.minPoint.x < total_scene_bounding_box->minPoint.x)
				total_scene_bounding_box->minPoint.x = (*it)->bounding_box.minPoint.x;
			if ((*it)->bounding_box.minPoint.y < total_scene_bounding_box->minPoint.y)
				total_scene_bounding_box->minPoint.y = (*it)->bounding_box.minPoint.y;
			if ((*it)->bounding_box.minPoint.z < total_scene_bounding_box->minPoint.z)
				total_scene_bounding_box->minPoint.z = (*it)->bounding_box.minPoint.z;

			if ((*it)->bounding_box.maxPoint.x > total_scene_bounding_box->maxPoint.x)
				total_scene_bounding_box->maxPoint.x = (*it)->bounding_box.maxPoint.x;
			if ((*it)->bounding_box.maxPoint.y > total_scene_bounding_box->maxPoint.y)
				total_scene_bounding_box->maxPoint.y = (*it)->bounding_box.maxPoint.y;
			if ((*it)->bounding_box.maxPoint.z > total_scene_bounding_box->maxPoint.z)
				total_scene_bounding_box->maxPoint.z = (*it)->bounding_box.maxPoint.z;
		}

		//now we have to get the biggest size of the model

		FocusCamera();

		aiReleaseImport(scene);
	}
	else
	{
		App->UI->console->AddLog("file not found");
	}
	
	return;
}
void ModuleMeshLoader::FocusCamera()
{

	float distance_x = (((total_scene_bounding_box->maxPoint.x - total_scene_bounding_box->minPoint.x) / 2) / tan(20 * DEGTORAD)) * cos(45 * DEGTORAD);
	float distance_y = (((total_scene_bounding_box->maxPoint.y - total_scene_bounding_box->minPoint.y) / 2) / tan(20 * DEGTORAD)) * sin(45 * DEGTORAD);
	float distance_z = (((total_scene_bounding_box->maxPoint.z - total_scene_bounding_box->minPoint.z) / 2) / tan(20 * DEGTORAD)) * sin(45 * DEGTORAD);

	App->camera->Look({ distance_x,distance_y,distance_z }, { total_scene_bounding_box->CenterPoint().x,total_scene_bounding_box->CenterPoint().y, total_scene_bounding_box->CenterPoint().z });
}
void NOTmesh::draw()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);


	if (App->renderer3D->draw_checkers)
	{
		glBindTexture(GL_TEXTURE_2D, App->renderer3D->texture_buffer);
	}	
	else if (texture != 0)
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

void ModuleMeshLoader::DrawSceneBoundingBox()
{
	glBegin(GL_LINES);

	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->maxPoint.z);
	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->maxPoint.z);
	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->maxPoint.z);

	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->maxPoint.z);
	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->minPoint.z);
	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->maxPoint.z);
	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->minPoint.z);

	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->minPoint.z);
	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->minPoint.z);
	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->minPoint.z);
	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->minPoint.z);

	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->minPoint.z);
	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->maxPoint.z);
	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->minPoint.z);
	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->maxPoint.z);

	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->maxPoint.z);

	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->maxPoint.z);
	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->maxPoint.z);

	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->maxPoint.z);
	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->minPoint.z);

	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->minPoint.z);
	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->minPoint.z);

	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->minPoint.z);
	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->maxPoint.z);

	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->minPoint.y, total_scene_bounding_box->maxPoint.z);
	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->maxPoint.z);


	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->maxPoint.z);
	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->maxPoint.z);

	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->maxPoint.z);
	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->minPoint.z);

	glVertex3f(total_scene_bounding_box->minPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->minPoint.z);
	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->minPoint.z);

	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->minPoint.z);
	glVertex3f(total_scene_bounding_box->maxPoint.x, total_scene_bounding_box->maxPoint.y, total_scene_bounding_box->maxPoint.z);


	glEnd();
}

NOTmesh::~NOTmesh()
{
	if (tex_coords != nullptr)
	{
		delete[] tex_coords;
		tex_coords = nullptr;
	}
	if (vertex != nullptr)
	{
		delete[] vertex;
		vertex = nullptr;
	}
	if (index != nullptr)
	{
		delete[] index;
		index = nullptr;
	}

	glDeleteBuffers(1 , &buffer_id);
	if (texture != 0)
		glDeleteTextures(1, &texture);

	//delete &scaling;
	//delete &rotation;
	//delete &position;

}