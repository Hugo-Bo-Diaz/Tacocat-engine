#include "ModuleFileSystem.h"
#include "Application.h"


ModuleFileSystem::ModuleFileSystem(bool start_enabled)
{
	listentoevents = true;

	windowpos.x = 100;
	windowpos.y = 100;
}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Start()
{
	return true;
}

bool ModuleFileSystem::CleanUp()
{
	return true;
}

void ModuleFileSystem::Save(rapidjson::Document * d, rapidjson::Value * v)
{

}

void ModuleFileSystem::Load(rapidjson::Value & v)
{
}

uint ModuleFileSystem::AddResource(Mesh * mesh, const char* path)
{
	Resource* r = new Resource();
	r->mesh.ptr = mesh;
	r->type = RES_MESH;
	r->path = path;

	resources.push_back(r);

	return r->UID;
}

uint ModuleFileSystem::AddResource(Material * mat, const char* path)
{

	Resource* r = new Resource();
	r->mat.ptr = mat;
	r->type = RES_MATERIAL;
	r->path = path;

	resources.push_back(r);

	return r->UID;

}

Resource * ModuleFileSystem::ResourceFromUID(uint UID)
{
	for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); it++)
	{
		if ((*it)->UID == UID)
			return (*it);
	}
	return nullptr;
}

Resource * ModuleFileSystem::ResourceFromPath(const char * path)
{
	for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); it++)
	{
		std::string temp = path;
		if ((*it)->path == temp)
			return (*it);
	}
	return nullptr;
}

Resource * ModuleFileSystem::LoadFile(const char * path)
{
	for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); it++)
	{
		if ((*it)->path == path)
			return (*it);
	}
	//if it hasn't returned the original resource loaded there it means its new

	Resource* r = new Resource();
	r->type = RES_INVALID;

	std::string extension = std::strrchr(path, '.');
	if (extension == ".FBX" || extension == ".fbx")
	{
		App->mesh_loader->Load_mesh(path, App->scene_controller->current_scene);
		r->path = path;
	}
	else if (extension == ".dds" || extension == ".tga" || extension == ".png")
	{
		Material* m = new Material();
		m->texture_buffer_id = App->tex_loader->LoadTexture(path, &m->tex_width, &m->tex_height);
		r->type = RES_MATERIAL;
		r->mat.ptr = m;
		r->path = path;
		resources.push_back(r);
	}

	return r;
}

void ModuleFileSystem::LoadResourcesInfo()
{
}

void ModuleFileSystem::GenerateResourcesInfo(rapidjson::Document * d, rapidjson::Value * v)
{
	for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); it++)
	{

		rapidjson::Document::AllocatorType& all = d->GetAllocator();

		rapidjson::Value res_obj(rapidjson::kObjectType);

		res_obj.AddMember("UID", (*it)->UID, all);
		//res_obj.AddMember("original path",(*it)->path,all);
		//res_obj.AddMember("library path", (*it)->path_in_library, all);
		res_obj.AddMember("type", (uint)(*it)->type, all);
		//TODO GET THE FILE DATE AND OTHER INFO that should be in metadata

		v->AddMember("Resource", res_obj, all);

		//LPSYSTEMTIME t;
		//FileTimeToSystemTime(&(*it)->last_modified,t);
		////in t we have the last modification data
		////CreateFile(,);
		//DWORD d;
		//FILE f;
		////OpenFile
		
		

	}
}

void ModuleFileSystem::DrawUI()
{
	//ImGui::SetNextWindowPos(windowpos, ImGuiSetCond_FirstUseEver);
	//ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);
	//ImGui::Begin("Properties", &window_active);
	//if (ImGui::CollapsingHeader("Model"))
	//{
	//	if (App->scene_controller->current_scene->spookamera->selected != nullptr)
	//	{
	//		for (std::list<Component*>::iterator it = App->scene_controller->current_scene->spookamera->selected->components.begin(); it != App->scene_controller->current_scene->spookamera->selected->components.end(); it++)
	//		{
	//			(*it)->Properties();
	//		}
	//	}
	//}
	//ImGui::End();

}


void ModuleFileSystem::RecieveEvent(Event& ev) 
{
	if (ev.type == Event::file_dropped)
	{
		LoadFile(ev.string.ptr);
	}
}


Resource::Resource()
{
	UID = App->random_int(0, 100000);
}

Resource::Resource(uint _UID)
{
	UID = _UID;
}

void Resource::Generate_in_Library()
{
	if (mesh.ptr != nullptr)
	{
		switch ((uint)type)
		{
		case RES_MESH:
			App->importer->mesh->CreateOwnFile(mesh.ptr);
			break;
		case RES_MATERIAL:
			;
			break;
		case RES_AUDIO:
			App->UI->console->AddLog("audio not implemented");
			break;
		case RES_INVALID:
			App->UI->console->AddLog("not a valid resource type");
		default:
			break;
		}
	}
}
