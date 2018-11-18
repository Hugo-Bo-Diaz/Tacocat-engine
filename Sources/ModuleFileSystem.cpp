#include "ModuleFileSystem.h"
#include "Application.h"

#include <filesystem>



ModuleFileSystem::ModuleFileSystem(bool start_enabled)
{
	listentoevents = true;

	windowpos.x = 100;
	windowpos.y = 100;

	//current_directory= SDL_GetBasePath();

	GetFilesFromDirectory(current_directory.c_str());

	Createdir("./Library");
	Createdir("./Library/Materials");
	Createdir("./Library/Meshes");
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
	mesh->Resource_UID = r->UID;

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
		m = App->tex_loader->LoadTexture(path);
		r->type = RES_MATERIAL;
		r->mat.ptr = m;
		r->path = path;
		r->path_in_library = m->path_in_library;
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
		res_obj.AddMember("original path", (rapidjson::Value::StringRefType)(*it)->path.c_str(),all);
		res_obj.AddMember("library path", (rapidjson::Value::StringRefType)(*it)->path_in_library.c_str(), all);
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
	std::string prev_currdir = current_directory;

	ImGui::SetNextWindowPos(windowpos, ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Assets", &window_active);

	if (ImGui::Button("ROOT"))
	{
		current_directory = default_directory;
	}


	for (std::vector<Display*>::iterator it = files.begin(); it != files.end();++it)
	{
		if ((*it)->isFolder)
		{
			if (ImGui::Button((*it)->path.c_str()))
			{
				current_directory = (*it)->path.c_str();
			}
		}
		else
		{
			if (ImGui::Button((*it)->path.c_str()))
			{
				LoadFile((*it)->path.c_str());
			}
		}

	}
	if (prev_currdir != current_directory)
	{
		GetFilesFromDirectory(current_directory.c_str());
	}

	ImGui::End();

}


void ModuleFileSystem::RecieveEvent(Event& ev) 
{
	if (ev.type == Event::file_dropped)
	{
		LoadFile(ev.string.ptr);
	}
}

void ModuleFileSystem::GetFilesFromDirectory(const char * directory)
{
	//DELETE THE DIRECTORIES
	//for (auto& p : std::experimental::filesystem::recursive_directory_iterator(directory))
	//	if (p.status().type() == std::experimental::filesystem::file_type::directory)
	//		r.push_back(p.path().string());
	for (std::vector<Display*>::iterator it = files.begin(); it != files.end(); ++it)
	{
		delete(*it);
	}
	files.clear();

	const std::experimental::filesystem::directory_iterator end{};

	for (std::experimental::filesystem::directory_iterator iter{ directory }; iter != end; ++iter)
	{
		Display* d = new Display();
		d->path = iter->path().string();
		files.push_back(d);

		if (std::experimental::filesystem::is_directory(*iter))
		{
			d->isFolder = true;
		}

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

void ModuleFileSystem::Createdir(const std::string& path)
{
	DWORD ftyp = GetFileAttributesA(path.c_str());
	LPCSTR file = path.c_str();
	if (ftyp == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(file, NULL);//Doesn't exist
		SetFileAttributes(file, FILE_ATTRIBUTE_HIDDEN);
	}
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY);
	//It exists
}