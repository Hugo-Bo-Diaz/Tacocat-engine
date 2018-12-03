#include "ModuleAudio.h"

ModuleAudio::ModuleAudio()
{
}


ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Init()
{
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		return false;
	}
	return false;
}
