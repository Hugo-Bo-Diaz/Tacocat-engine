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
bool ModuleTextureLoader::Start()
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


uint ModuleTextureLoader::LoadTexture(const char* file, uint* _width,  uint* _height)
{
	ILuint buffernumber = 0;

	bool textureLoaded = false;

	std::string initial = file;

	std::string nameoffile = "";
	if (initial.find('\\',0) != -1)
		nameoffile = std::strrchr(initial.c_str(), '\\');


	//Generate and set current image ID
	ilGenImages(1, &buffernumber);
	ilBindImage(buffernumber);
	
	ILboolean success;
	if (nameoffile != "")
	{
		std::string fin = "street" + nameoffile;//HARDCODED: NEEDS TO BE RESOURCED SYSTEM'D
		success = ilLoadImage(fin.c_str());
	}
	else
		success = ilLoadImage(file);

	//Image loaded successfully
	if (success == IL_TRUE)
	{
		////Convert image to RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();
	
		ILuint width = ilGetInteger(IL_IMAGE_WIDTH);
		*_width = width;
		ILuint height = ilGetInteger(IL_IMAGE_HEIGHT);
		*_height = height;
		ILbyte* pixmap = new ILbyte[width * height * 4];
		ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA,
			IL_UNSIGNED_BYTE, pixmap);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &buffernumber);
		glBindTexture(GL_TEXTURE_2D, buffernumber);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, pixmap);
		glBindTexture(GL_TEXTURE_2D, 0);

		delete[] pixmap;
		pixmap = nullptr;

		//Delete file from memory
		ilDeleteImages(1, &buffernumber);

		App->UI->console->AddLog("Loaded %s\n", file);

		texture_id = buffernumber;
		tex_width = width;
		tex_height = height;
	}
	else
	{
		App->UI->console->AddLog("Unable to load %s\n",file);

	}

	return buffernumber;
}