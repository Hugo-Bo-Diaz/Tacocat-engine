#include "Globals.h"
#include "Application.h"
#include "ModuleMeshLoader.h"

#include "MeshComponent.h"
#include "MaterialComponent.h"

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
	listentoevents = true;
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

void ModuleMeshLoader::Load_mesh(const char* file, Scene* scene_to)//TODO, RECIEVE SCENE TO LOAD TO
{
	if (total_scene_bounding_box != nullptr)
	{
		delete total_scene_bounding_box;
		total_scene_bounding_box = nullptr;
	}

	total_scene_bounding_box = new AABB({100,100,100}, {-100,-100,-100});

	GameObject* parent = App->scene_controller->current_scene->AddGameObject();

	const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		Load_node(scene->mRootNode,parent,scene);

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

void ModuleMeshLoader::Load_node(aiNode * node, GameObject * parent,const aiScene* scene)
{
	GameObject* par = new GameObject();
	parent->AddChild(par);

	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		//GameObject* Object = new GameObject();
		//par->AddChild(Object);
		Component_Mesh* m_comp = new Component_Mesh();
		Mesh* m = new Mesh();
		m_comp->mesh = m;
		par->AddComponent(m_comp);

		aiMesh* iterator = scene->mMeshes[node->mMeshes[i]];

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
				{
					App->UI->console->AddLog("geometry messed up");
					m->not_working = true;
				}

				else
					memcpy(&m->index[i * 3], iterator->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
		if (iterator->HasTextureCoords(0))
		{
			m->tex_coords = new float[m->num_vertex * 2];
			uint w = 0;
			for (uint i = 0; i < iterator->mNumVertices * 2; i += 2)
			{
				memcpy(&m->tex_coords[i], &iterator->mTextureCoords[0][w].x, sizeof(float));
				memcpy(&m->tex_coords[i + 1], &iterator->mTextureCoords[0][w].y, sizeof(float));
				++w;
			}
		}

		if (iterator->HasNormals())
		{
			m->num_normals = iterator->mNumVertices;
			m->normals = new float[m->num_normals * 3];
			memcpy(m->normals, iterator->mNormals, sizeof(float)*m->num_normals * 3);
		}

		m->bounding_box = m->bounding_box.MinimalEnclosingAABB((float3*)m->vertex, m->num_vertex);


		uint material_index = iterator->mMaterialIndex;

		par->name = iterator->mName.C_Str();

		//parent->AddComponent(TRANSFORM) @DANI

		Component_Transform* transform = new Component_Transform();
		par->AddComponent(transform);

		node->mTransformation.Decompose(transform->scaling,transform->rotation, transform->position);
		transform->rotation_angle = transform->rotation.GetEuler();

		float3 p,s;
		p.x = transform->position.x;
		p.y = transform->position.y;
		p.z = transform->position.z;

		s.x = transform->scaling.x;
		s.y = transform->scaling.y;
		s.z = transform->scaling.z;

		Quat r;
		r.x = transform->rotation.x;
		r.y = transform->rotation.y;
		r.z = transform->rotation.z;
		r.w = transform->rotation.w;
		

		transform->transform_local = float4x4::FromTRS(p,r,s);

		m->bounding_box.Scale(m->bounding_box.CenterPoint(),(transform->scaling.x, transform->scaling.y, transform->scaling.z));

		//m->bounding_box.Translate(float3( transform->position.x,transform->position.y,transform->position.z ));

		Component_Material* mat = new Component_Material();
		par->AddComponent((Component*)mat);
		mat->material_index = material_index;
		//m->material = mat;

		const aiMaterial* material = scene->mMaterials[material_index];
		aiString texturePath;

		unsigned int numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);   // always 0

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
			mat->texture_buffer_id = App->tex_loader->LoadTexture(texturePath.C_Str(), &(mat->tex_width), &(mat->tex_height));
		}

		glGenBuffers(1, (GLuint*) &(m->buffer_id));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->buffer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*m->num_index, &m->index[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	

	//for (std::list<GameObject*>::iterator it = parent->children.begin(); it != parent->children.end(); it++)
	//{
	//	for (std::list<Component*>::iterator it_1 = (*it)->components.begin(); it_1 != (*it)->components.end(); it_1++)
	//	{
	//		if ((*it_1)->type == MESH && ((Component_Mesh*)(*it_1))->material_index != -1 && ((Component_Mesh*)(*it_1))->material_index < scene->mNumMaterials && scene->HasMaterials())
	//		{
	//			Component_Material* mat = new Component_Material();
	//			(*it)->AddComponent((Component*)mat);
	//			mat->material_index = ((Component_Mesh*)(*it_1))->material_index;

	//			//now we know which texture does this mesh need and we add it to the game object as a component
	//			const aiMaterial* material = scene->mMaterials[((Component_Mesh*)(*it_1))->material_index];
	//			aiString texturePath;

	//			unsigned int numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);   // always 0

	//			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	//			{
	//				material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
	//				mat->texture_buffer_id = App->tex_loader->LoadTexture(texturePath.C_Str(), &(mat->tex_width), &(mat->tex_height));
	//			}
	//		}
	//	}
	//}

	if (node->mNumChildren > 0)
	{
		for (int i = 0; i < node->mNumChildren; ++i)
		{
			Load_node(node->mChildren[i],par,scene);
		}
	}
	
}

void ModuleMeshLoader::FocusCamera()
{

	//float distance_x = (((total_scene_bounding_box->maxPoint.x - total_scene_bounding_box->minPoint.x) / 2) / tan(20 * DEGTORAD)) * cos(45 * DEGTORAD);
	//float distance_y = (((total_scene_bounding_box->maxPoint.y - total_scene_bounding_box->minPoint.y) / 2) / tan(20 * DEGTORAD)) * sin(45 * DEGTORAD);
	//float distance_z = (((total_scene_bounding_box->maxPoint.z - total_scene_bounding_box->minPoint.z) / 2) / tan(20 * DEGTORAD)) * sin(45 * DEGTORAD);

	//App->scene_controller->current_scene->spookamera->->Look({ distance_x,distance_y,distance_z }, { total_scene_bounding_box->CenterPoint().x,total_scene_bounding_box->CenterPoint().y, total_scene_bounding_box->CenterPoint().z });
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


void ModuleMeshLoader::RecieveEvent(Event& ev)
{

}
