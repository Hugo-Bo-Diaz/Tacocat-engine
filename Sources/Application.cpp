#include "Application.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/reader.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"


Application::Application()
{

	window = new ModuleWindow(true);
	input = new ModuleInput(true);
	audio = new ModuleAudio(true);
	scene_controller = new ModuleSceneController(true);
	mesh_loader = new ModuleMeshLoader(true);
	renderer3D = new ModuleRenderer3D(true);
	//camera = new ModuleCamera3D(true);
	//physics = new ModulePhysics3D(true);
	UI = new ModuleUI(true);
	tex_loader = new ModuleTextureLoader(true);
	importer = new ModuleImporter(true);
	fsys = new ModuleFileSystem(true);
	

	//pcg32_srandom_r(&rng1, time(NULL), (intptr_t)&rng1);
	//pcg32_srandom_r(&rng2, time(NULL), (intptr_t)&rng2);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order
	// Main Modules
	AddModule(window);
	//AddModule(camera);
	AddModule(input);
	AddModule(mesh_loader);
	AddModule(tex_loader);
	AddModule(importer);
	AddModule(fsys);

	// Scenes
	AddModule(audio);
	AddModule(scene_controller);

		// UI
	AddModule(UI);

	// Renderer last!
	AddModule(renderer3D);


}

Application::~Application()
{
	//std::list<Module*>::iterator item = list_modules.end();

	for (std::list<Module*>::reverse_iterator it = list_modules.rbegin(); it != list_modules.rend(); it++)
	{
		//list_modules.remove(*it);
		delete (*it);
	}

	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	LoadConfig("config.json");

	// Call Init() in all modules

	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == true; it++)
	{
		ret = (*it)->Init();
	}

	// After all Init calls we call Start() in all modules
	App->UI->console->AddLog("Application Start --------------");

	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == true; it++)
	{
		ret = (*it)->Start();
	}
	ms_timer.Start();	


	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	float frame_time = ms_timer.Read();
	dt = (frame_time / 1000.0f) / custom_dt;
	UI->send_FPSdata(1/dt, frame_time);
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	float frame_time = ms_timer.Read();
	float del = 1000 / confg_fps - frame_time;
	if (del > 0)
		SDL_Delay(del);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	if(App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		SaveConfig("config.json");
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
		LoadConfig("config.json");

	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == UPDATE_CONTINUE; it++)
	{
		ret = (*it)->PreUpdate(dt);
	}

	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == UPDATE_CONTINUE; it++)
	{
		ret = (*it)->Update(dt);
	}

	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == UPDATE_CONTINUE; it++)
	{
		ret = (*it)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	SaveConfig("config.json");

	for (std::list<Module*>::reverse_iterator it = list_modules.rbegin(); it != list_modules.rend(); it++)
		if (!(*it)->CleanUp()) return false;

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::Close()
{
	quit = true;
}

int Application::random_int(int min, int max)
{
	return (int)pcg32_boundedrand_r(&rng1, max - min) + min;
}

float Application::random_between_0_1()
{
	return ldexp(pcg32_random_r(&rng2), -32);
}

void Application::SaveConfig(const char* filename)
{
	rapidjson::Document document;
	document.SetObject();
	FILE* fp = fopen(filename, "wb");
	char writeBuffer[655360];

	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Document::AllocatorType& all = document.GetAllocator();

	rapidjson::Value modules_object(rapidjson::kObjectType);
	modules_object.AddMember("modules",list_modules.size(),all);

	rapidjson::Value module_obj(rapidjson::kObjectType);

	module_obj.AddMember("FPS", confg_fps, all);
	module_obj.AddMember("vsync", confg_vsync, all);

	modules_object.AddMember("App", module_obj, all);

	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); it++)
	{
		(*it)->Save(&document, &modules_object);
	}

	document.AddMember("App", modules_object, all);
	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	document.Accept(writer);
	fclose(fp);
}

void Application::LoadConfig(const char* filename)
{

	rapidjson::Document file;

	FILE* f = fopen(filename,"rb");
	if (f)
	{
		char Buffer[65536];
		rapidjson::FileReadStream input(f, Buffer, sizeof(Buffer));
		file.ParseStream(input);

		rapidjson::Value& modules = file["App"];

		rapidjson::Value& conf = modules["App"];

		confg_fps = conf["FPS"].GetFloat();
		confg_vsync = conf["vsync"].GetBool();

		for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); it++)
		{
			(*it)->Load(modules);
		}

	}
	else
	{
		App->UI->console->AddLog("couldn't find the file");
	}

}

void Application::BroadcastEvent(Event& ev)
{
	for (std::list<Module*>::iterator it_mod = list_modules.begin(); it_mod != list_modules.end(); it_mod++)
	{
		if ((*it_mod)->listentoevents)
			(*it_mod)->RecieveEvent(ev);
	}
}