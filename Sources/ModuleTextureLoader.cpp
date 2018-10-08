#include "Globals.h"
#include "Application.h"
#include "ModuleTextureLoader.h"
#include "Primitive.h"
#include "Devil/include/il.h"
#include "Devil/include/ilut.h"


#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#pragma comment( lib, "Devil/libx86/ILUT.lib" )
//#include "parson\parson.h"

ModuleTextureLoader::ModuleTextureLoader(bool start_enabled) : Module(start_enabled)
{
	name = "Texture Loader";

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

ModuleTextureLoader::~ModuleTextureLoader()
{
	ilShutDown();
}

// Load assets
bool ModuleTextureLoader::Init()
{
	bool ret = true;



	return ret;
}

// Load assets
bool ModuleTextureLoader::CleanUp()
{
	App->UI->console->AddLog("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleTextureLoader::Update(float dt)
{


	return UPDATE_CONTINUE;
}

uint ModuleTextureLoader::LoadTexture(const char* file)
{
	uint buffernumber = -1;



	return buffernumber;
}