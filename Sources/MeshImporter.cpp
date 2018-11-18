#include "MeshImporter.h"
#include "MeshComponent.h"
#include <experimental/filesystem>
#include "GameObject.h"
#include "Scene.h"
#include "Application.h"
#include "ModuleSceneController.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


void MeshImporter::CreateOwnFile(Mesh* mesh)
{

	if (mesh->not_working)
	{
		return;
	}
	//need to save: index, vertex, texcoords and normals IN THAT ORDER

	uint sizeofname = 0;
	if (mesh->material != nullptr)
		sizeofname = sizeof(char) *mesh->material->path_in_library.size();

	uint hastexcoords = 0;
	uint sizeoftexcoords = 0;
	if (mesh->tex_coords != nullptr)
	{
		sizeoftexcoords = sizeof(float) * mesh->num_vertex * 2;
		hastexcoords = 1;
	}

	uint ranges[4] = { mesh->num_index , mesh->num_vertex, sizeofname, hastexcoords};
	
	uint size = sizeof(ranges) + sizeof(uint) * mesh->num_index + sizeof(float) * mesh->num_vertex * 3 + sizeoftexcoords + sizeofname;//TODO NORMALS

	char* data = new char[size]; // Allocate
	char* cursor = data;

	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);

	cursor += bytes; // Store indices

	bytes = sizeof(uint) * mesh->num_index;
	memcpy(cursor, mesh->index, bytes);

	cursor += bytes; // Store vertices

	bytes = sizeof(float) * mesh->num_vertex * 3;
	memcpy(cursor, mesh->vertex, bytes);

	if (mesh->tex_coords != nullptr)
	{
		cursor += bytes;// Store texcoords
		bytes = sizeof(float) * mesh->num_vertex * 2;
		memcpy(cursor, mesh->tex_coords, bytes);
	}

	if (mesh->material != nullptr)
	{
		cursor += bytes;
		bytes = sizeofname;	
		memcpy(cursor, &mesh->material->path_in_library[0], bytes);
	}

	std::string name = "Library//Meshes//";
	name += std::to_string(mesh->Resource_UID);
	name += ".taco";

	FILE* file = fopen(name.c_str(),"wb");
	fwrite(data,sizeof(char),size,file);
	fclose(file);
}

void MeshImporter::Import(uint mesh_resource_uid)
{
	std::string path = "Library//Meshes//";

	namespace stdfs = std::experimental::filesystem;

	const stdfs::directory_iterator end{};

	for (stdfs::directory_iterator iter{ path.data() }; iter != end; ++iter)
	{
		if (stdfs::is_regular_file(*iter))
			filenames.push_back(iter->path().string());
	}


}

void MeshImporter::LoadCustomMeshFiles(Scene* scene_to)
{
	for (std::vector<std::string>::iterator it = filenames.begin();it != filenames.end();++it)
	{
		//each file is a separate mesh
		GameObject* object = scene_to->AddGameObject();
		Component_Mesh* mesh_comp = new Component_Mesh();
		Mesh* mesh = new Mesh();
		mesh_comp->mesh = mesh;

		object->AddComponent(mesh_comp);

		FILE* file = fopen(it->data(),"rb");
		if (file != NULL)
		{
			//mesh->name = (*it);//TODO GET THE NAME FROM HERE

			fseek(file, 0, SEEK_END);
			uint size = ftell(file);
			rewind(file);
			char* buffer = new char[size];
			fread(buffer, 1, size, file);

			char* cursor = buffer;
			uint ranges[4];
			uint bytes = sizeof(uint) * 4;

			memcpy(ranges, buffer, bytes);

			mesh->num_index = ranges[0];
			mesh->num_vertex = ranges[1];
			uint name_length = ranges[2];
			uint has_texcoords = ranges[3];

			// Load indices
			cursor += bytes;
			
			bytes = sizeof(uint) * mesh->num_index;
			mesh->index = new uint[mesh->num_index];
			memcpy(mesh->index, cursor, bytes);

			// Load vertex
			cursor += bytes;

			bytes = sizeof(float) * mesh->num_vertex * 3;
			mesh->vertex = new float[mesh->num_vertex * 3];
			memcpy(mesh->vertex, cursor,bytes);

			// Load texcoords
			if (has_texcoords != 0)
			{
				cursor += bytes;

				bytes = sizeof(float) * mesh->num_vertex * 2;
				mesh->tex_coords = new float[mesh->num_vertex * 2];
				memcpy(mesh->tex_coords, cursor, bytes);
			}

			cursor += bytes;

			bytes = sizeof(char) * name_length;
			std::string nameinlibrary(cursor, cursor + bytes);

			fclose(file);
			
			//App->UI->console->AddLog("Loaded  %s", mesh->name.c_str());

			mesh->bounding_box = mesh->bounding_box.MinimalEnclosingAABB((float3*)mesh->vertex, mesh->num_vertex);
			//mesh->bounding_box.minPoint = float3(-0.5, -0.5, -0.5);
			//mesh->bounding_box.maxPoint = float3(0.5, 0.5, 0.5);
			if (nameinlibrary.size() > 0)
			{
				Resource* r = App->fsys->LoadFile(nameinlibrary.c_str());
				mesh->material = r->mat.ptr;
				mesh->materialUID = r->UID;
			}


			//GENERATE BUFFER
			glGenBuffers(1, (GLuint*) &(mesh->buffer_id));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffer_id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*mesh->num_index, &mesh->index[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		
	}
	filenames.clear();
}
