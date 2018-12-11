#include "ModuleAudio.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory and stream managers

ModuleAudio::ModuleAudio(bool start_enabled)
{
	Init();
}


ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Init()
{
	//
   // Create and initialize an instance of the default memory manager. Note
   // that you can override the default memory manager with your own. Refer
   // to the SDK documentation for more information.
   //
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		App->UI->console->AddLog("Could not create the memory manager.");
		assert(!"Could not create the memory manager.");
		return false;
	}

	return false;
}
